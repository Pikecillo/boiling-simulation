#pragma once

#include <vector>

template <typename T>
class Matrix {
public:
 Matrix(size_t rows, size_t cols) :
  m_elements(rows * cols, static_cast<T>(0)),
    m_rows(rows), m_cols(cols) {}
  
  void operator=(const Matrix &other);
  
  size_t cols() const { return m_cols; }

  size_t rows() const { return m_rows; }

  double at(size_t i, size_t j) const {
    return m_elements[i * m_cols + j];
  }
  
  double& at(size_t i, size_t j) {
    return m_elements[i * m_cols + j];
  }
  
private:
  size_t m_cols, m_rows;
  std::vector<T> m_elements;
};

template <typename T>
void Matrix<T>::operator=(const Matrix<T> &matrix) {
  m_cols = matrix.m_cols;
  m_rows = matrix.m_rows;
  m_elements = matrix.m_elements;
}

using Matrixd = Matrix<double>;
