#pragma once

#include <vector>

template <typename T> class Matrix {
public:
  Matrix(size_t rows, size_t cols)
      : m_elements(rows * cols, static_cast<T>(0)), m_rows(rows), m_cols(cols) {
  }
  
  size_t cols() const { return m_cols; }

  size_t rows() const { return m_rows; }

  double at(size_t i, size_t j) const { return m_elements[i * m_cols + j]; }

  double &at(size_t i, size_t j) { return m_elements[i * m_cols + j]; }

private:
  size_t m_cols, m_rows;
  std::vector<T> m_elements;
};
  
using Matrixd = Matrix<double>;
