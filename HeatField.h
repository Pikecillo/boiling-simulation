#pragma once

#include <cmath>

#include "Matrix.h"

class HeatField {
public:
  HeatField(size_t height, size_t width);

  size_t height() const { return m_heat.rows(); }

  size_t width() const { return m_heat.cols(); }

  double at(size_t i, size_t j) const { return m_heat.at(i, j); }

  void update();

private:
  void heat_diffusion_update(const Matrixd &field0, Matrixd &field);
  void buoyancy_update(const Matrixd &field0, Matrixd &field);
  void latent_heat_update(const Matrixd &field0, Matrixd &field);
  void surface_tension_update(const Matrixd &field0, Matrixd &field);

  double phase_function(double T);
  void normal(const Matrixd &heat, int x, int y, double n[]);
  void add_vectors(double a, double v1[], double b, double v2[], double v[]);
  double mean_curvature(const Matrixd &heat, int x, int y);
  double curvature(const Matrixd &heat, double px, double py, double nx,
                   double ny);
  double module(double v[]);

private:
  static constexpr double epsilon = 0.3;
  static constexpr double alpha = 1.7;
  static constexpr double buoyancy = 0.5;
  static constexpr double Tc = 10.0;
  static constexpr double latent_heat = 0.1;
  static constexpr double rho = 0.00;

  Matrixd m_heat, m_heat0, m_heat1;
};
