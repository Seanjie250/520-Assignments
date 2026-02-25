# HW3: DynamicArray Implementation

This assignment implements a DynamicArray Abstract Data Type (ADT) in C with various mathematical and utility operations.

## Files

- `dynamic_array.h` - Header file with function prototypes
- `dynamic_array.c` - Implementation of all DynamicArray methods
- `test_dynamic_array.c` - Comprehensive test suite
- `main.c` - Demo program
- `Makefile` - Build configuration

## Building

```bash
make
```

This builds:
- `test_dynamic_array` - Test executable
- `main` - Demo program

## Running Tests

```bash
make test
```

Or directly:
```bash
./test_dynamic_array
```

## Running Demo

```bash
./main
```

## Implemented Methods

### Mathematical Operations
- `DynamicArray_min()` - Returns minimum value (asserts non-empty)
- `DynamicArray_max()` - Returns maximum value (asserts non-empty)
- `DynamicArray_mean()` - Returns average value (asserts non-empty)
- `DynamicArray_median()` - Returns median value (asserts non-empty)
- `DynamicArray_sum()` - Returns sum of all values

### Access Methods
- `DynamicArray_first()` - Returns first element (asserts non-empty)
- `DynamicArray_last()` - Returns last element (asserts non-empty)

### Array Operations
- `DynamicArray_copy()` - Creates a copy of an array
- `DynamicArray_range(start, end, step)` - Creates array with values from start to end with step
- `DynamicArray_concat(arr1, arr2)` - Concatenates two arrays
- `DynamicArray_take(arr, n)` - Takes n elements (positive from start, negative from end, pads with zeros)
- `DynamicArray_filter(arr, pred)` - Filters array using a predicate function
- `DynamicArray_unique(arr)` - Removes duplicates, preserves order
- `DynamicArray_split(arr, n_chunks, &actual_chunks)` - Splits array into n chunks

### Extra Credit: Array Tracking
- `DynamicArray_num_arrays()` - Returns number of allocated arrays
- `DynamicArray_destroy_all()` - Destroys all allocated arrays
- `DynamicArray_is_valid(arr)` - Checks if array is still valid (not destroyed)

## Example Usage

```c
DynamicArray *arr = DynamicArray_new(10);
DynamicArray_append(arr, 5.0);
DynamicArray_append(arr, 2.0);
DynamicArray_append(arr, 8.0);

printf("Min: %.1f\n", DynamicArray_min(arr));
printf("Max: %.1f\n", DynamicArray_max(arr));

DynamicArray *range = DynamicArray_range(0, 1, 0.1);
DynamicArray *filtered = DynamicArray_filter(arr, is_positive);

DynamicArray_destroy(arr);
DynamicArray_destroy(range);
DynamicArray_destroy(filtered);
```

## Clean

```bash
make clean
```
