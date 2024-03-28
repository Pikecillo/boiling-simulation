#pragma once

#include <cmath>

#include "Matrix.h"

class HeatField
{
  public:
    HeatField(size_t height, size_t width);

    size_t height() const
    {
        return m_heat.rows();
    }

    size_t width() const
    {
        return m_heat.cols();
    }

    float at(size_t i, size_t j) const
    {
        return m_heat.at(i, j);
    }

    void update();

  private:
    void heat_diffusion_update(const Matrixd &field0, Matrixd &field);
    void buoyancy_update(const Matrixd &field0, Matrixd &field);
    void latent_heat_update(const Matrixd &field0, Matrixd &field);
    void surface_tension_update(const Matrixd &field0, Matrixd &field);

    float phase_function(float T);
    void normal(const Matrixd &heat, int x, int y, float n[]);
    void add_vectors(float a, float v1[], float b, float v2[], float v[]);
    float mean_curvature(const Matrixd &heat, int x, int y);
    float curvature(const Matrixd &heat, float px, float py, float nx, float ny);
    float module(float v[]);

  private:
    static constexpr float epsilon = 0.3;
    static constexpr float alpha = 1.7;
    static constexpr float buoyancy = 0.5;
    static constexpr float Tc = 10.0;
    static constexpr float latent_heat = 0.1;
    static constexpr float rho = 0.00;

    Matrixd m_heat, m_heat0, m_heat1;
};
