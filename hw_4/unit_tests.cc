#include "typed_array.h"
#include "matrix.h"
#include <cassert>
#include <iostream>
#include <cmath>

#define EPSILON 1e-9

void test_typed_array_push_pop() {
    TypedArray<int> arr;
    arr.push(1);
    arr.push(2);
    arr.push(3);
    
    assert(arr.size() == 3);
    assert(arr.get(0) == 1);
    assert(arr.get(1) == 2);
    assert(arr.get(2) == 3);
    
    assert(arr.pop() == 3);
    assert(arr.size() == 2);
    
    bool threw_exception = false;
    try {
        TypedArray<int> empty;
        empty.pop();
    } catch (const std::range_error& e) {
        assert(std::string(e.what()) == "Cannot pop from an empty array");
        threw_exception = true;
    }
    assert(threw_exception);
    
    std::cout << "test_typed_array_push_pop: PASSED\n";
}

void test_typed_array_push_front_pop_front() {
    TypedArray<int> arr;
    arr.push_front(3);
    arr.push_front(2);
    arr.push_front(1);
    
    assert(arr.size() == 3);
    assert(arr.get(0) == 1);
    assert(arr.get(1) == 2);
    assert(arr.get(2) == 3);
    
    assert(arr.pop_front() == 1);
    assert(arr.size() == 2);
    assert(arr.get(0) == 2);
    
    bool threw_exception = false;
    try {
        TypedArray<int> empty;
        empty.pop_front();
    } catch (const std::range_error& e) {
        assert(std::string(e.what()) == "Cannot pop from an empty array");
        threw_exception = true;
    }
    assert(threw_exception);
    
    std::cout << "test_typed_array_push_front_pop_front: PASSED\n";
}

void test_typed_array_concat() {
    TypedArray<int> a;
    a.set(0, 0);
    a.set(1, 1);
    
    TypedArray<int> b = a.concat(a).concat(a);
    assert(b.size() == 6);
    assert(b.get(0) == 0);
    assert(b.get(1) == 1);
    assert(b.get(2) == 0);
    assert(b.get(3) == 1);
    assert(b.get(4) == 0);
    assert(b.get(5) == 1);
    
    assert(a.size() == 2);
    assert(a.get(0) == 0);
    assert(a.get(1) == 1);
    
    std::cout << "test_typed_array_concat: PASSED\n";
}

void test_typed_array_reverse() {
    TypedArray<int> arr;
    arr.push(1);
    arr.push(2);
    arr.push(3);
    
    TypedArray<int>& reversed = arr.reverse();
    assert(&reversed == &arr);
    assert(arr.get(0) == 3);
    assert(arr.get(1) == 2);
    assert(arr.get(2) == 1);
    
    std::cout << "test_typed_array_reverse: PASSED\n";
}

void test_typed_array_operator_plus() {
    TypedArray<int> a;
    a.set(0, 0);
    a.set(1, 1);
    
    TypedArray<int> b = a + a + a;
    assert(b.size() == 6);
    assert(b.get(0) == 0);
    assert(b.get(1) == 1);
    assert(b.get(2) == 0);
    assert(b.get(3) == 1);
    assert(b.get(4) == 0);
    assert(b.get(5) == 1);
    
    std::cout << "test_typed_array_operator_plus: PASSED\n";
}

void test_matrix_constructors() {
    Matrix m1;
    assert(m1.isEmpty());
    assert(m1.rows() == 0);
    assert(m1.cols() == 0);
    
    Matrix m2(2, 3);
    assert(m2.rows() == 2);
    assert(m2.cols() == 3);
    assert(!m2.isEmpty());
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            assert(std::abs(m2(i, j)) < EPSILON);
        }
    }
    
    Matrix m3(2, 2, 5.0);
    assert(m3.rows() == 2);
    assert(m3.cols() == 2);
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            assert(std::abs(m3(i, j) - 5.0) < EPSILON);
        }
    }
    
    Matrix m4 = {{1, 2}, {3, 4}};
    assert(m4.rows() == 2);
    assert(m4.cols() == 2);
    assert(std::abs(m4(0, 0) - 1.0) < EPSILON);
    assert(std::abs(m4(0, 1) - 2.0) < EPSILON);
    assert(std::abs(m4(1, 0) - 3.0) < EPSILON);
    assert(std::abs(m4(1, 1) - 4.0) < EPSILON);
    
    bool threw_exception = false;
    try {
        Matrix m5 = {{1, 2}, {3}};
    } catch (const std::invalid_argument&) {
        threw_exception = true;
    }
    assert(threw_exception);
    
    std::cout << "test_matrix_constructors: PASSED\n";
}

void test_matrix_copy() {
    Matrix m1 = {{1, 2}, {3, 4}};
    Matrix m2(m1);
    
    assert(m2 == m1);
    m2(0, 0) = 99;
    assert(std::abs(m1(0, 0) - 1.0) < EPSILON);
    assert(std::abs(m2(0, 0) - 99.0) < EPSILON);
    
    Matrix m3 = m1;
    assert(m3 == m1);
    m3(1, 1) = 88;
    assert(std::abs(m1(1, 1) - 4.0) < EPSILON);
    assert(std::abs(m3(1, 1) - 88.0) < EPSILON);
    
    std::cout << "test_matrix_copy: PASSED\n";
}

void test_matrix_access() {
    Matrix m = {{1, 2}, {3, 4}};
    
    assert(std::abs(m(0, 0) - 1.0) < EPSILON);
    m(0, 0) = 10;
    assert(std::abs(m(0, 0) - 10.0) < EPSILON);
    
    const Matrix cm = {{5, 6}, {7, 8}};
    assert(std::abs(cm(1, 1) - 8.0) < EPSILON);
    
    assert(std::abs(m.at(1, 1) - 4.0) < EPSILON);
    m.at(1, 1) = 20;
    assert(std::abs(m.at(1, 1) - 20.0) < EPSILON);
    
    bool threw_exception = false;
    try {
        m.at(10, 10);
    } catch (const std::out_of_range&) {
        threw_exception = true;
    }
    assert(threw_exception);
    
    std::cout << "test_matrix_access: PASSED\n";
}

void test_matrix_arithmetic() {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{5, 6}, {7, 8}};
    
    Matrix C = A + B;
    assert(std::abs(C(0, 0) - 6.0) < EPSILON);
    assert(std::abs(C(0, 1) - 8.0) < EPSILON);
    assert(std::abs(C(1, 0) - 10.0) < EPSILON);
    assert(std::abs(C(1, 1) - 12.0) < EPSILON);
    
    Matrix D = A - B;
    assert(std::abs(D(0, 0) - (-4.0)) < EPSILON);
    
    Matrix E = A * B;
    assert(std::abs(E(0, 0) - 19.0) < EPSILON);
    assert(std::abs(E(0, 1) - 22.0) < EPSILON);
    assert(std::abs(E(1, 0) - 43.0) < EPSILON);
    assert(std::abs(E(1, 1) - 50.0) < EPSILON);
    
    Matrix F = 2.0 * A;
    assert(std::abs(F(0, 0) - 2.0) < EPSILON);
    assert(std::abs(F(1, 1) - 8.0) < EPSILON);
    
    Matrix G = A * 3.0;
    assert(std::abs(G(0, 0) - 3.0) < EPSILON);
    
    Matrix H = A / 2.0;
    assert(std::abs(H(0, 0) - 0.5) < EPSILON);
    
    Matrix I = -A;
    assert(std::abs(I(0, 0) - (-1.0)) < EPSILON);
    
    bool threw_exception = false;
    try {
        Matrix wrong = A + Matrix(3, 3);
    } catch (const std::invalid_argument&) {
        threw_exception = true;
    }
    assert(threw_exception);
    
    std::cout << "test_matrix_arithmetic: PASSED\n";
}

void test_matrix_compound_assignment() {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{1, 1}, {1, 1}};
    
    A += B;
    assert(std::abs(A(0, 0) - 2.0) < EPSILON);
    
    A -= B;
    assert(std::abs(A(0, 0) - 1.0) < EPSILON);
    
    A *= 2.0;
    assert(std::abs(A(0, 0) - 2.0) < EPSILON);
    
    A /= 2.0;
    assert(std::abs(A(0, 0) - 1.0) < EPSILON);
    
    Matrix C = {{1, 2}, {3, 4}};
    Matrix D = {{1, 0}, {0, 1}};
    C *= D;
    assert(C == Matrix({{1, 2}, {3, 4}}));
    
    std::cout << "test_matrix_compound_assignment: PASSED\n";
}

void test_matrix_comparison() {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{1, 2}, {3, 4}};
    Matrix C = {{1, 2}, {3, 5}};
    
    assert(A == B);
    assert(A != C);
    assert(!(A == C));
    
    std::cout << "test_matrix_comparison: PASSED\n";
}

void test_matrix_transpose() {
    Matrix A = {{1, 2, 3}, {4, 5, 6}};
    Matrix AT = A.transpose();
    
    assert(AT.rows() == 3);
    assert(AT.cols() == 2);
    assert(std::abs(AT(0, 0) - 1.0) < EPSILON);
    assert(std::abs(AT(0, 1) - 4.0) < EPSILON);
    assert(std::abs(AT(1, 0) - 2.0) < EPSILON);
    assert(std::abs(AT(1, 1) - 5.0) < EPSILON);
    assert(std::abs(AT(2, 0) - 3.0) < EPSILON);
    assert(std::abs(AT(2, 1) - 6.0) < EPSILON);
    
    Matrix ATT = AT.transpose();
    assert(ATT == A);
    
    std::cout << "test_matrix_transpose: PASSED\n";
}

void test_matrix_trace() {
    Matrix A = {{1, 2}, {3, 4}};
    double tr = A.trace();
    assert(std::abs(tr - 5.0) < EPSILON);
    
    Matrix I = Matrix::identity(3);
    assert(std::abs(I.trace() - 3.0) < EPSILON);
    
    bool threw_exception = false;
    try {
        Matrix non_square(2, 3);
        non_square.trace();
    } catch (const std::logic_error&) {
        threw_exception = true;
    }
    assert(threw_exception);
    
    std::cout << "test_matrix_trace: PASSED\n";
}

void test_matrix_diagonal() {
    Matrix A = {{1, 2, 3}, {4, 5, 6}};
    Matrix diag = A.diagonal();
    
    assert(diag.rows() == 2);
    assert(diag.cols() == 1);
    assert(std::abs(diag(0, 0) - 1.0) < EPSILON);
    assert(std::abs(diag(1, 0) - 5.0) < EPSILON);
    
    std::cout << "test_matrix_diagonal: PASSED\n";
}

void test_matrix_norm() {
    Matrix A = {{3, 4}};
    double n = A.norm();
    assert(std::abs(n - 5.0) < EPSILON);
    
    Matrix B = {{1, 2}, {3, 4}};
    double n2 = B.norm();
    double expected = std::sqrt(1 + 4 + 9 + 16);
    assert(std::abs(n2 - expected) < EPSILON);
    
    std::cout << "test_matrix_norm: PASSED\n";
}

void test_matrix_static_factories() {
    Matrix I = Matrix::identity(3);
    assert(I.isSquare());
    assert(std::abs(I(0, 0) - 1.0) < EPSILON);
    assert(std::abs(I(1, 1) - 1.0) < EPSILON);
    assert(std::abs(I(2, 2) - 1.0) < EPSILON);
    assert(std::abs(I(0, 1)) < EPSILON);
    
    Matrix Z = Matrix::zeros(2, 3);
    assert(Z.rows() == 2);
    assert(Z.cols() == 3);
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            assert(std::abs(Z(i, j)) < EPSILON);
        }
    }
    
    Matrix O = Matrix::ones(2, 3);
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            assert(std::abs(O(i, j) - 1.0) < EPSILON);
        }
    }
    
    std::vector<double> diag_vals = {1, 2, 3};
    Matrix D = Matrix::diagonal(diag_vals);
    assert(D.isSquare());
    assert(D.rows() == 3);
    assert(std::abs(D(0, 0) - 1.0) < EPSILON);
    assert(std::abs(D(1, 1) - 2.0) < EPSILON);
    assert(std::abs(D(2, 2) - 3.0) < EPSILON);
    assert(std::abs(D(0, 1)) < EPSILON);
    
    std::cout << "test_matrix_static_factories: PASSED\n";
}

void test_matrix_properties() {
    Matrix A = {{1, 2}, {3, 4}};
    assert(A.isSquare());
    assert(!A.isEmpty());
    
    Matrix B(0, 0);
    assert(B.isEmpty());
    assert(!B.isSquare());
    
    Matrix C(2, 3);
    assert(!C.isSquare());
    assert(!C.isEmpty());
    
    std::cout << "test_matrix_properties: PASSED\n";
}

void test_matrix_mathematical_properties() {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix zero = Matrix::zeros(2, 2);
    
    Matrix A_plus_zero = A + zero;
    assert(A_plus_zero == A);
    
    Matrix I = Matrix::identity(2);
    assert(std::abs(I.trace() - 2.0) < EPSILON);
    
    Matrix AT = A.transpose();
    Matrix ATT = AT.transpose();
    assert(ATT == A);
    
    std::cout << "test_matrix_mathematical_properties: PASSED\n";
}

int main() {
    std::cout << "Running HW4 tests...\n\n";
    
    test_typed_array_push_pop();
    test_typed_array_push_front_pop_front();
    test_typed_array_concat();
    test_typed_array_reverse();
    test_typed_array_operator_plus();
    
    test_matrix_constructors();
    test_matrix_copy();
    test_matrix_access();
    test_matrix_arithmetic();
    test_matrix_compound_assignment();
    test_matrix_comparison();
    test_matrix_transpose();
    test_matrix_trace();
    test_matrix_diagonal();
    test_matrix_norm();
    test_matrix_static_factories();
    test_matrix_properties();
    test_matrix_mathematical_properties();
    
    std::cout << "\nAll tests PASSED!\n";
    return 0;
}
