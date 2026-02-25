# HW4: TypedArray and Matrix Implementation

This assignment implements a TypedArray template class and a Matrix class in C++.

## Files

- `typed_array.h` - TypedArray template class implementation
- `matrix.h` - Matrix class header
- `matrix.cc` - Matrix class implementation
- `unit_tests.cc` - Comprehensive test suite
- `main.cc` - Demo program
- `Makefile` - Build configuration

## Building

```bash
make
```

This builds:
- `unit_tests` - Test executable
- `main` - Demo program

## Running Tests

```bash
make test
```

Or directly:
```bash
./unit_tests
```

## Running Demo

```bash
./main
```

## TypedArray Implementation

### Methods Added

- `push(const ElementType& value)` - Add element to end
- `pop()` - Remove and return last element (throws `std::range_error` if empty)
- `push_front(const ElementType& value)` - Add element to beginning
- `pop_front()` - Remove and return first element (throws `std::range_error` if empty)
- `concat(const TypedArray& other)` - Returns new concatenated array
- `reverse()` - Reverses array in place, returns reference to self
- `operator+(const TypedArray& other)` - Concatenation operator

### Example Usage

```cpp
TypedArray<int> a;
a.set(0, 0);
a.set(1, 1);

TypedArray<int> b = a + a + a;  // [0, 1, 0, 1, 0, 1]
a.reverse();  // Reverses a in place
```

## Matrix Implementation

### Constructors

- `Matrix()` - Empty 0×0 matrix
- `Matrix(rows, cols)` - Zero-initialized matrix
- `Matrix(rows, cols, value)` - Matrix filled with value
- `Matrix({{...}, {...}})` - From initializer list
- `Matrix(const Matrix&)` - Copy constructor (deep copy)

### Element Access

- `operator()(row, col)` - Non-const and const versions
- `at(row, col)` - With bounds checking (throws `std::out_of_range`)

### Arithmetic Operators

- `+`, `-`, `*` (matrix multiplication), `*` (scalar), `/`, `-` (unary)
- `+=`, `-=`, `*=`, `/=`
- `==`, `!=`

### Matrix Operations

- `transpose()` - Returns transpose
- `trace()` - Sum of diagonal (square matrices only)
- `diagonal()` - Extract diagonal as column vector
- `fill(value)` - Fill entire matrix
- `norm()` - Frobenius norm

### Static Factory Methods

- `Matrix::identity(n)` - n×n identity matrix
- `Matrix::zeros(rows, cols)` - Zero matrix
- `Matrix::ones(rows, cols)` - Matrix of ones
- `Matrix::diagonal(vector)` - Diagonal matrix from vector

### Example Usage

```cpp
Matrix A = {{1, 2}, {3, 4}};
Matrix B = {{5, 6}, {7, 8}};

Matrix C = A + B;           // Addition
Matrix D = A * B;           // Matrix multiplication
Matrix E = 2.0 * A;         // Scalar multiplication
Matrix F = A.transpose();   // Transpose
double tr = A.trace();      // Trace
double n = A.norm();        // Norm
```

## Test Coverage

All tests pass (18/18):
- TypedArray: push/pop, push_front/pop_front, concat, reverse, operator+
- Matrix: constructors, copy semantics, access, arithmetic, compound assignment, comparison
- Matrix operations: transpose, trace, diagonal, norm
- Static factories: identity, zeros, ones, diagonal
- Edge cases: empty matrices, dimension mismatches, exceptions
- Mathematical properties: (A^T)^T = A, A + 0 = A, trace(I_n) = n

## Clean

```bash
make clean
```
