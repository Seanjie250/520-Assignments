#include "typed_array.h"
#include "matrix.h"
#include <iostream>

int main() {
    std::cout << "=== HW4 Demo ===\n\n";
    
    std::cout << "--- TypedArray Demo ---\n";
    TypedArray<int> arr;
    arr.push(1);
    arr.push(2);
    arr.push(3);
    std::cout << "After push: size = " << arr.size() << "\n";
    
    arr.push_front(0);
    std::cout << "After push_front(0): size = " << arr.size() << "\n";
    
    TypedArray<int> arr2 = arr + arr;
    std::cout << "After concat: size = " << arr2.size() << "\n";
    
    arr.reverse();
    std::cout << "After reverse: first element = " << arr.get(0) << "\n";
    
    std::cout << "\n--- Matrix Demo ---\n";
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{5, 6}, {7, 8}};
    
    std::cout << "Matrix A:\n";
    for (size_t i = 0; i < A.rows(); i++) {
        for (size_t j = 0; j < A.cols(); j++) {
            std::cout << A(i, j) << " ";
        }
        std::cout << "\n";
    }
    
    Matrix C = A + B;
    std::cout << "\nA + B:\n";
    for (size_t i = 0; i < C.rows(); i++) {
        for (size_t j = 0; j < C.cols(); j++) {
            std::cout << C(i, j) << " ";
        }
        std::cout << "\n";
    }
    
    Matrix D = A * B;
    std::cout << "\nA * B:\n";
    for (size_t i = 0; i < D.rows(); i++) {
        for (size_t j = 0; j < D.cols(); j++) {
            std::cout << D(i, j) << " ";
        }
        std::cout << "\n";
    }
    
    Matrix E = 2.0 * A;
    std::cout << "\n2 * A:\n";
    for (size_t i = 0; i < E.rows(); i++) {
        for (size_t j = 0; j < E.cols(); j++) {
            std::cout << E(i, j) << " ";
        }
        std::cout << "\n";
    }
    
    Matrix F = A.transpose();
    std::cout << "\nA^T:\n";
    for (size_t i = 0; i < F.rows(); i++) {
        for (size_t j = 0; j < F.cols(); j++) {
            std::cout << F(i, j) << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\nTrace of A: " << A.trace() << "\n";
    std::cout << "Norm of A: " << A.norm() << "\n";
    
    Matrix I = Matrix::identity(3);
    std::cout << "\n3x3 Identity matrix trace: " << I.trace() << "\n";
    
    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
