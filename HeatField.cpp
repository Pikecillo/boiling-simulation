#include "HeatField.h"

HeatField::HeatField(size_t height, size_t width)
    : m_heat(height, width), m_heat0(height, width), m_heat1(height, width) {
  const float high = 9.9;
  const float low = 2.0;
  const float medium = 2.0;

  for (size_t i = 0; i < m_heat.rows(); i++)
    for (size_t j = 0; j < m_heat.cols(); j++) {
      float heat;
      if (i == 0)
        heat = 0.99 * high * fabs(sin(j / 20.0));
      else
        heat = (i == (m_heat0.rows() - 1) ? low : medium);

      m_heat.at(i, j) = m_heat0.at(i, j) = m_heat1.at(i, j) = heat;
    }
}

void HeatField::update() {
  m_heat0 = m_heat;

  // Account for heat diffusion using five point Laplacian
  heat_diffusion_update(m_heat0, m_heat1);
  // Account for buoyancy forces in vertical direction
  buoyancy_update(m_heat1, m_heat);

  // Account for latent heat release
  // This is the heat resulting from the conversion from water
  // to steam and from steam to water
  // It also accounts for the surface tension
  // as in the extended yanagita
  latent_heat_update(m_heat0, m_heat);

  m_heat1 = m_heat;

  // Account for surface tension
  // Part of the extended Yanagita model
  surface_tension_update(m_heat1, m_heat);
}

void HeatField::heat_diffusion_update(const Matrixd &field0, Matrixd &field) {
  const float quarter_diffusion = 0.25 * epsilon;

  for (size_t i = 1; i + 1 < field0.rows(); i++)
    for (size_t j = 1; j + 1 < field0.cols(); j++) {

      if (i == 0 || i == field0.rows() - 1)
        field.at(i, j) = field0.at(i, j);
      else
        field.at(i, j) =
            field0.at(i, j) +
            quarter_diffusion * (field0.at(i, j - 1) + field0.at(i, j + 1) +
                                 field0.at(i - 1, j) + field0.at(i + 1, j) -
                                 4.0 * field0.at(i, j));
    }
}

void HeatField::buoyancy_update(const Matrixd &field0, Matrixd &field) {
  const float half_buoyancy = 0.5 * buoyancy;

  for (size_t i = 1; i + 1 < field0.rows(); i++)
    for (size_t j = 1; j + 1 < field0.cols(); j++) {
      field.at(i, j) =
          field0.at(i, j) *
          (1 - half_buoyancy * (phase_function(field0.at(i + 1, j)) -
                                phase_function(field0.at(i - 1, j))));
    }
}

void HeatField::latent_heat_update(const Matrixd &field0, Matrixd &field) {
  for (size_t i = 1; i < field0.rows(); i++)
    for (size_t j = 1; j + 1 < field0.cols(); j++) {

      const bool to_steam = (field0.at(i, j) <= Tc && field.at(i, j) > Tc);
      const bool to_water = (field0.at(i, j) > Tc && field.at(i, j) <= Tc);

      field.at(i, j) +=
          ((to_steam ? latent_heat : 0) - (to_water ? latent_heat : 0));
    }
}

void HeatField::surface_tension_update(const Matrixd &field0, Matrixd &field) {
  for (size_t i = 0; i < field0.rows(); i++)
    for (size_t j = 0; j < field0.cols(); j++) {
      // If there is a change in the sign of phase then
      // the point is on the interface
      if (phase_function(field0.at(i, j)) *
                  phase_function(field0.at(i, j - 1)) <
              0.0 ||
          phase_function(field0.at(i, j)) *
                  phase_function(field0.at(i, j + 1)) <
              0.0 ||
          phase_function(field0.at(i, j)) *
                  phase_function(field0.at(i - 1, j)) <
              0.0 ||
          phase_function(field0.at(i, j)) *
                  phase_function(field0.at(i + 1, j)) <
              0.0) {

        field.at(i, j) += (rho * mean_curvature(field0, i, j));
      }
    }
}

float HeatField::phase_function(float T) { return tanh(alpha * (T - Tc)); }

void HeatField::normal(const Matrixd &heat, int x, int y, float n[]) {
  n[0] = heat.at(y, x) - heat.at(y, x - 1);
  n[1] = heat.at(y, x) - heat.at(y - 1, x);
}

void HeatField::add_vectors(float a, float v1[], float b, float v2[],
                            float v[]) {
  v[0] = a * v1[0] + b * v2[0];
  v[1] = a * v1[1] + b * v2[1];
}

float HeatField::mean_curvature(const Matrixd &heat, int x, int y) {
  float sum = 0.0;

  sum += curvature(heat, x, y, x - 1, y);
  sum += curvature(heat, x, y, x + 1, y);
  sum += curvature(heat, x, y, x, y - 1);
  sum += curvature(heat, x, y, x, y + 1);

  return 0.25 * sum;
}

float HeatField::module(float v[]) { return sqrt(v[0] * v[0] + v[1] * v[1]); }

float HeatField::curvature(const Matrixd &heat, float px, float py,
                            float nx, float ny) {
  float kpn, np[2], nn[2], p[] = {px, py}, n[] = {nx, ny}, t1[2], t2[2], t3[2],
                            t4[2];

  // Calculate normal at p and n
  normal(heat, px, py, np);
  normal(heat, nx, ny, nn);

  add_vectors(1.0, np, -1.0, nn, t1); // t1 = np - nn
  add_vectors(1.0, p, -1.0, n, t2);   // t2 = p - n
  add_vectors(1.0, np, 1.0, p, t3);   // t3 = np + p
  add_vectors(1.0, np, 1.0, n, t4);   // t4 = np + n

  kpn = module(t1) / module(t2);

  if (module(t2) <= module(t3) - module(t4))
    kpn *= -1.0;

  return kpn;
}
