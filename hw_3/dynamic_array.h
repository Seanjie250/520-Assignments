#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <assert.h>

typedef struct DynamicArray {
    double *buffer;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* DynamicArray_new(size_t initial_capacity);
void DynamicArray_destroy(DynamicArray *arr);
void DynamicArray_append(DynamicArray *arr, double value);
double DynamicArray_get(const DynamicArray *arr, size_t index);
void DynamicArray_set(DynamicArray *arr, size_t index, double value);
size_t DynamicArray_size(const DynamicArray *arr);

double DynamicArray_min(const DynamicArray *arr);
double DynamicArray_max(const DynamicArray *arr);
double DynamicArray_mean(const DynamicArray *arr);
double DynamicArray_median(const DynamicArray *arr);
double DynamicArray_sum(const DynamicArray *arr);

double DynamicArray_first(const DynamicArray *arr);
double DynamicArray_last(const DynamicArray *arr);

DynamicArray* DynamicArray_copy(const DynamicArray *arr);
DynamicArray* DynamicArray_range(double start, double end, double step);
DynamicArray* DynamicArray_concat(const DynamicArray *arr1, const DynamicArray *arr2);
DynamicArray* DynamicArray_take(const DynamicArray *arr, int n);

typedef int (*Predicate)(double);
DynamicArray* DynamicArray_filter(const DynamicArray *arr, Predicate pred);
DynamicArray* DynamicArray_unique(const DynamicArray *arr);
DynamicArray** DynamicArray_split(const DynamicArray *arr, int n_chunks, int *actual_chunks);

int DynamicArray_num_arrays(void);
void DynamicArray_destroy_all(void);
int DynamicArray_is_valid(const DynamicArray *arr);

#endif
