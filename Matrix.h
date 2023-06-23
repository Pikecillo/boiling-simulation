#pragma once

#include <vector>

template <typename T> class Matrix {
public:
  Matrix(size_t rows, size_t cols)
      : m_cols(cols), m_rows(rows), m_elements(rows * cols, static_cast<T>(0)) {
  }
  
  size_t cols() const { return m_cols; }

  size_t rows() const { return m_rows; }

  float at(size_t i, size_t j) const { return m_elements[i * m_cols + j]; }

  float &at(size_t i, size_t j) { return m_elements[i * m_cols + j]; }

  size_t size() const { return m_elements.size(); };

private:
  size_t m_cols, m_rows;
  std::vector<T> m_elements;
};
  
using Matrixd = Matrix<float>;
