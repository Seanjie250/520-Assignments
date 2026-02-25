#include "matrix.h"
#include <algorithm>

Matrix::Matrix() : num_rows(0), num_cols(0) {
}

Matrix::Matrix(size_t rows, size_t cols) 
    : num_rows(rows), num_cols(cols) {
    data.resize(rows * cols, 0.0);
}

Matrix::Matrix(size_t rows, size_t cols, double value) 
    : num_rows(rows), num_cols(cols) {
    data.resize(rows * cols, value);
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) {
    if (list.size() == 0) {
        num_rows = 0;
        num_cols = 0;
        return;
    }
    
    num_rows = list.size();
    num_cols = list.begin()->size();
    
    for (const auto& row : list) {
        if (row.size() != num_cols) {
            throw std::invalid_argument("All rows must have the same number of columns");
        }
    }
    
    data.reserve(num_rows * num_cols);
    for (const auto& row : list) {
        for (double val : row) {
            data.push_back(val);
        }
    }
}

Matrix::Matrix(const Matrix& other) 
    : data(other.data), num_rows(other.num_rows), num_cols(other.num_cols) {
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        data = other.data;
        num_rows = other.num_rows;
        num_cols = other.num_cols;
    }
    return *this;
}

double& Matrix::operator()(size_t row, size_t col) {
    return data[index(row, col)];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    return data[index(row, col)];
}

double& Matrix::at(size_t row, size_t col) {
    if (row >= num_rows || col >= num_cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data[index(row, col)];
}

const double& Matrix::at(size_t row, size_t col) const {
    if (row >= num_rows || col >= num_cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data[index(row, col)];
}

size_t Matrix::rows() const {
    return num_rows;
}

size_t Matrix::cols() const {
    return num_cols;
}

bool Matrix::isEmpty() const {
    return num_rows == 0 || num_cols == 0;
}

bool Matrix::isSquare() const {
    return num_rows == num_cols && num_rows > 0;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    Matrix result(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    Matrix result(num_rows, num_cols);
    for (size_t i = 0; i < data.size(); i++) {
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (num_cols != other.num_rows) {
        throw std::invalid_argument("Matrix dimensions must be compatible for multiplication");
    }
    
    Matrix result(num_rows, other.num_cols);
    for (size_t i = 0; i < num_rows; i++) {
        for (size_t j = 0; j < other.num_cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < num_cols; k++) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(*this);
    for (size_t i = 0; i < data.size(); i++) {
        result.data[i] *= scalar;
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

Matrix Matrix::operator/(double scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::invalid_argument("Division by zero");
    }
    return *this * (1.0 / scalar);
}

Matrix Matrix::operator-() const {
    return *this * -1.0;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    *this = *this * other;
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] *= scalar;
    }
    return *this;
}

Matrix& Matrix::operator/=(double scalar) {
    *this = *this / scalar;
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (num_rows != other.num_rows || num_cols != other.num_cols) {
        return false;
    }
    
    for (size_t i = 0; i < data.size(); i++) {
        if (std::abs(data[i] - other.data[i]) >= EPSILON) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

Matrix Matrix::transpose() const {
    Matrix result(num_cols, num_rows);
    for (size_t i = 0; i < num_rows; i++) {
        for (size_t j = 0; j < num_cols; j++) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

double Matrix::trace() const {
    if (!isSquare()) {
        throw std::logic_error("Trace is only defined for square matrices");
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < num_rows; i++) {
        sum += (*this)(i, i);
    }
    return sum;
}

Matrix Matrix::diagonal() const {
    size_t diag_size = std::min(num_rows, num_cols);
    Matrix result(diag_size, 1);
    for (size_t i = 0; i < diag_size; i++) {
        result(i, 0) = (*this)(i, i);
    }
    return result;
}

void Matrix::fill(double value) {
    std::fill(data.begin(), data.end(), value);
}

double Matrix::norm() const {
    double sum_sq = 0.0;
    for (double val : data) {
        sum_sq += val * val;
    }
    return std::sqrt(sum_sq);
}

Matrix Matrix::identity(size_t n) {
    Matrix result(n, n);
    for (size_t i = 0; i < n; i++) {
        result(i, i) = 1.0;
    }
    return result;
}

Matrix Matrix::zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols, 0.0);
}

Matrix Matrix::ones(size_t rows, size_t cols) {
    return Matrix(rows, cols, 1.0);
}

Matrix Matrix::diagonal(const std::vector<double>& diag) {
    size_t n = diag.size();
    Matrix result(n, n, 0.0);
    for (size_t i = 0; i < n; i++) {
        result(i, i) = diag[i];
    }
    return result;
}
