#include "dynamic_array.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EPSILON 1e-9

int is_positive(double x) {
    return x > 0;
}

void test_min_max() {
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, 5.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 8.0);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 9.0);
    
    assert(fabs(DynamicArray_min(arr) - 1.0) < EPSILON);
    assert(fabs(DynamicArray_max(arr) - 9.0) < EPSILON);
    
    DynamicArray_destroy(arr);
    printf("test_min_max: PASSED\n");
}

void test_mean_median() {
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 3.0);
    DynamicArray_append(arr, 4.0);
    DynamicArray_append(arr, 5.0);
    
    assert(fabs(DynamicArray_mean(arr) - 3.0) < EPSILON);
    assert(fabs(DynamicArray_median(arr) - 3.0) < EPSILON);
    
    DynamicArray_append(arr, 6.0);
    assert(fabs(DynamicArray_median(arr) - 3.5) < EPSILON);
    
    DynamicArray_destroy(arr);
    printf("test_mean_median: PASSED\n");
}

void test_sum() {
    DynamicArray *arr = DynamicArray_new(10);
    assert(fabs(DynamicArray_sum(arr)) < EPSILON);
    
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 3.0);
    
    assert(fabs(DynamicArray_sum(arr) - 6.0) < EPSILON);
    
    DynamicArray_destroy(arr);
    printf("test_sum: PASSED\n");
}

void test_first_last() {
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, 10.0);
    DynamicArray_append(arr, 20.0);
    DynamicArray_append(arr, 30.0);
    
    assert(fabs(DynamicArray_first(arr) - 10.0) < EPSILON);
    assert(fabs(DynamicArray_last(arr) - 30.0) < EPSILON);
    
    DynamicArray_destroy(arr);
    printf("test_first_last: PASSED\n");
}

void test_copy() {
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 3.0);
    
    DynamicArray *copy = DynamicArray_copy(arr);
    assert(copy != arr);
    assert(copy->size == arr->size);
    for (size_t i = 0; i < arr->size; i++) {
        assert(fabs(DynamicArray_get(copy, i) - DynamicArray_get(arr, i)) < EPSILON);
    }
    
    DynamicArray_destroy(arr);
    DynamicArray_destroy(copy);
    printf("test_copy: PASSED\n");
}

void test_range() {
    DynamicArray *arr = DynamicArray_range(0, 1, 0.1);
    assert(arr->size == 11);
    assert(fabs(DynamicArray_get(arr, 0) - 0.0) < EPSILON);
    assert(fabs(DynamicArray_get(arr, 10) - 1.0) < EPSILON);
    
    DynamicArray_destroy(arr);
    printf("test_range: PASSED\n");
}

void test_concat() {
    DynamicArray *a = DynamicArray_range(0, 1, 0.1);
    DynamicArray *b = DynamicArray_range(1.1, 2, 0.1);
    DynamicArray *c = DynamicArray_concat(a, b);
    
    assert(c->size == a->size + b->size);
    assert(fabs(DynamicArray_get(c, 0) - 0.0) < EPSILON);
    double last_val = DynamicArray_get(c, c->size - 1);
    assert(fabs(last_val - 2.0) < 0.01);
    
    DynamicArray_destroy(a);
    DynamicArray_destroy(b);
    DynamicArray_destroy(c);
    printf("test_concat: PASSED\n");
}

void test_take() {
    DynamicArray *a = DynamicArray_range(1, 5, 1);
    DynamicArray *b = DynamicArray_take(a, 2);
    DynamicArray *c = DynamicArray_take(a, -2);
    DynamicArray *d = DynamicArray_take(a, 7);
    
    assert(b->size == 2);
    assert(fabs(DynamicArray_get(b, 0) - 1.0) < EPSILON);
    assert(fabs(DynamicArray_get(b, 1) - 2.0) < EPSILON);
    
    assert(c->size == 2);
    assert(fabs(DynamicArray_get(c, 0) - 4.0) < EPSILON);
    assert(fabs(DynamicArray_get(c, 1) - 5.0) < EPSILON);
    
    assert(d->size == 7);
    assert(fabs(DynamicArray_get(d, 0) - 1.0) < EPSILON);
    assert(fabs(DynamicArray_get(d, 4) - 5.0) < EPSILON);
    assert(fabs(DynamicArray_get(d, 5) - 0.0) < EPSILON);
    assert(fabs(DynamicArray_get(d, 6) - 0.0) < EPSILON);
    
    DynamicArray_destroy(a);
    DynamicArray_destroy(b);
    DynamicArray_destroy(c);
    DynamicArray_destroy(d);
    printf("test_take: PASSED\n");
}

void test_filter() {
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, -2.0);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, -1.0);
    DynamicArray_append(arr, 3.0);
    DynamicArray_append(arr, -5.0);
    
    DynamicArray *filtered = DynamicArray_filter(arr, is_positive);
    assert(filtered->size == 2);
    assert(fabs(DynamicArray_get(filtered, 0) - 1.0) < EPSILON);
    assert(fabs(DynamicArray_get(filtered, 1) - 3.0) < EPSILON);
    
    DynamicArray_destroy(arr);
    DynamicArray_destroy(filtered);
    printf("test_filter: PASSED\n");
}

void test_unique() {
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 3.0);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 4.0);
    DynamicArray_append(arr, 2.0);
    
    DynamicArray *unique = DynamicArray_unique(arr);
    assert(unique->size == 4);
    assert(fabs(DynamicArray_get(unique, 0) - 1.0) < EPSILON);
    assert(fabs(DynamicArray_get(unique, 1) - 2.0) < EPSILON);
    assert(fabs(DynamicArray_get(unique, 2) - 3.0) < EPSILON);
    assert(fabs(DynamicArray_get(unique, 3) - 4.0) < EPSILON);
    
    DynamicArray_destroy(arr);
    DynamicArray_destroy(unique);
    printf("test_unique: PASSED\n");
}

void test_split() {
    DynamicArray *arr = DynamicArray_range(1, 10, 1);
    int actual_chunks;
    DynamicArray **chunks = DynamicArray_split(arr, 3, &actual_chunks);
    
    assert(actual_chunks == 3);
    assert(chunks[0]->size == 4);
    assert(chunks[1]->size == 3);
    assert(chunks[2]->size == 3);
    
    for (int i = 0; i < actual_chunks; i++) {
        DynamicArray_destroy(chunks[i]);
    }
    free(chunks);
    DynamicArray_destroy(arr);
    printf("test_split: PASSED\n");
}

void test_num_arrays() {
    DynamicArray *a = DynamicArray_range(0, 1, 0.1);
    DynamicArray *b = DynamicArray_range(1.1, 2, 0.1);
    DynamicArray *c = DynamicArray_concat(a, b);
    (void)c;
    
    assert(DynamicArray_is_valid(a) == 1);
    assert(DynamicArray_num_arrays() == 3);
    
    DynamicArray_destroy_all();
    assert(DynamicArray_is_valid(a) == 0);
    assert(DynamicArray_num_arrays() == 0);
    
    printf("test_num_arrays: PASSED\n");
}

int main() {
    printf("Running DynamicArray tests...\n\n");
    
    test_min_max();
    test_mean_median();
    test_sum();
    test_first_last();
    test_copy();
    test_range();
    test_concat();
    test_take();
    test_filter();
    test_unique();
    test_split();
    test_num_arrays();
    
    printf("\nAll tests PASSED!\n");
    return 0;
}
