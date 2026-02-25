#include "dynamic_array.h"
#include <string.h>
#include <math.h>

static DynamicArray** allocated_arrays = NULL;
static size_t num_allocated = 0;
static size_t allocated_capacity = 0;

static void register_array(DynamicArray *arr) {
    if (allocated_arrays == NULL) {
        allocated_capacity = 16;
        allocated_arrays = malloc(allocated_capacity * sizeof(DynamicArray*));
    }
    
    if (num_allocated >= allocated_capacity) {
        allocated_capacity *= 2;
        allocated_arrays = realloc(allocated_arrays, allocated_capacity * sizeof(DynamicArray*));
    }
    
    allocated_arrays[num_allocated++] = arr;
}

static void unregister_array(DynamicArray *arr) {
    for (size_t i = 0; i < num_allocated; i++) {
        if (allocated_arrays[i] == arr) {
            allocated_arrays[i] = allocated_arrays[num_allocated - 1];
            num_allocated--;
            break;
        }
    }
}

DynamicArray* DynamicArray_new(size_t initial_capacity) {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    arr->capacity = initial_capacity > 0 ? initial_capacity : 1;
    arr->size = 0;
    arr->buffer = malloc(arr->capacity * sizeof(double));
    register_array(arr);
    return arr;
}

void DynamicArray_destroy(DynamicArray *arr) {
    if (arr == NULL) return;
    if (arr->buffer != NULL) {
        free(arr->buffer);
        arr->buffer = NULL;
    }
    unregister_array(arr);
    free(arr);
}

void DynamicArray_append(DynamicArray *arr, double value) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->buffer = realloc(arr->buffer, arr->capacity * sizeof(double));
    }
    arr->buffer[arr->size++] = value;
}

double DynamicArray_get(const DynamicArray *arr, size_t index) {
    return arr->buffer[index];
}

void DynamicArray_set(DynamicArray *arr, size_t index, double value) {
    arr->buffer[index] = value;
}

size_t DynamicArray_size(const DynamicArray *arr) {
    return arr->size;
}

double DynamicArray_min(const DynamicArray *arr) {
    assert(arr->size > 0);
    double min_val = arr->buffer[0];
    for (size_t i = 1; i < arr->size; i++) {
        if (arr->buffer[i] < min_val) {
            min_val = arr->buffer[i];
        }
    }
    return min_val;
}

double DynamicArray_max(const DynamicArray *arr) {
    assert(arr->size > 0);
    double max_val = arr->buffer[0];
    for (size_t i = 1; i < arr->size; i++) {
        if (arr->buffer[i] > max_val) {
            max_val = arr->buffer[i];
        }
    }
    return max_val;
}

double DynamicArray_mean(const DynamicArray *arr) {
    assert(arr->size > 0);
    return DynamicArray_sum(arr) / arr->size;
}

static int compare_double(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

double DynamicArray_median(const DynamicArray *arr) {
    assert(arr->size > 0);
    double *sorted = malloc(arr->size * sizeof(double));
    memcpy(sorted, arr->buffer, arr->size * sizeof(double));
    qsort(sorted, arr->size, sizeof(double), compare_double);
    
    double median;
    if (arr->size % 2 == 0) {
        median = (sorted[arr->size / 2 - 1] + sorted[arr->size / 2]) / 2.0;
    } else {
        median = sorted[arr->size / 2];
    }
    
    free(sorted);
    return median;
}

double DynamicArray_sum(const DynamicArray *arr) {
    double sum = 0.0;
    for (size_t i = 0; i < arr->size; i++) {
        sum += arr->buffer[i];
    }
    return sum;
}

double DynamicArray_first(const DynamicArray *arr) {
    assert(arr->size > 0);
    return arr->buffer[0];
}

double DynamicArray_last(const DynamicArray *arr) {
    assert(arr->size > 0);
    return arr->buffer[arr->size - 1];
}

DynamicArray* DynamicArray_copy(const DynamicArray *arr) {
    DynamicArray *copy = DynamicArray_new(arr->capacity);
    copy->size = arr->size;
    memcpy(copy->buffer, arr->buffer, arr->size * sizeof(double));
    return copy;
}

DynamicArray* DynamicArray_range(double start, double end, double step) {
    DynamicArray *arr = DynamicArray_new(16);
    if (step == 0.0) {
        return arr;
    }
    
    if (step > 0) {
        double val = start;
        while (val <= end + step * 0.5) {
            DynamicArray_append(arr, val);
            val += step;
        }
    } else {
        double val = start;
        while (val >= end + step * 0.5) {
            DynamicArray_append(arr, val);
            val += step;
        }
    }
    return arr;
}

DynamicArray* DynamicArray_concat(const DynamicArray *arr1, const DynamicArray *arr2) {
    DynamicArray *result = DynamicArray_new(arr1->size + arr2->size);
    result->size = arr1->size + arr2->size;
    
    memcpy(result->buffer, arr1->buffer, arr1->size * sizeof(double));
    memcpy(result->buffer + arr1->size, arr2->buffer, arr2->size * sizeof(double));
    
    return result;
}

DynamicArray* DynamicArray_take(const DynamicArray *arr, int n) {
    size_t abs_n = (size_t)abs(n);
    DynamicArray *result = DynamicArray_new(abs_n);
    result->size = abs_n;
    
    if (n > 0) {
        size_t take_count = abs_n < arr->size ? abs_n : arr->size;
        memcpy(result->buffer, arr->buffer, take_count * sizeof(double));
        for (size_t i = take_count; i < abs_n; i++) {
            result->buffer[i] = 0.0;
        }
    } else {
        size_t take_count = abs_n < arr->size ? abs_n : arr->size;
        size_t start_idx = arr->size - take_count;
        memcpy(result->buffer, arr->buffer + start_idx, take_count * sizeof(double));
        for (size_t i = take_count; i < abs_n; i++) {
            result->buffer[i] = 0.0;
        }
    }
    
    return result;
}

DynamicArray* DynamicArray_filter(const DynamicArray *arr, Predicate pred) {
    DynamicArray *result = DynamicArray_new(arr->size);
    for (size_t i = 0; i < arr->size; i++) {
        if (pred(arr->buffer[i])) {
            DynamicArray_append(result, arr->buffer[i]);
        }
    }
    return result;
}

DynamicArray* DynamicArray_unique(const DynamicArray *arr) {
    DynamicArray *result = DynamicArray_new(arr->size);
    for (size_t i = 0; i < arr->size; i++) {
        int found = 0;
        for (size_t j = 0; j < result->size; j++) {
            if (fabs(result->buffer[j] - arr->buffer[i]) < 1e-9) {
                found = 1;
                break;
            }
        }
        if (!found) {
            DynamicArray_append(result, arr->buffer[i]);
        }
    }
    return result;
}

DynamicArray** DynamicArray_split(const DynamicArray *arr, int n_chunks, int *actual_chunks) {
    if (n_chunks <= 0 || arr->size == 0) {
        *actual_chunks = 0;
        return NULL;
    }
    
    size_t chunk_size = arr->size / n_chunks;
    size_t remainder = arr->size % n_chunks;
    
    *actual_chunks = n_chunks;
    DynamicArray **chunks = malloc(n_chunks * sizeof(DynamicArray*));
    
    size_t idx = 0;
    for (int i = 0; i < n_chunks; i++) {
        size_t current_chunk_size = chunk_size + ((size_t)i < remainder ? 1 : 0);
        chunks[i] = DynamicArray_new(current_chunk_size);
        chunks[i]->size = current_chunk_size;
        
        for (size_t j = 0; j < current_chunk_size; j++) {
            chunks[i]->buffer[j] = arr->buffer[idx++];
        }
    }
    
    return chunks;
}

int DynamicArray_num_arrays(void) {
    return (int)num_allocated;
}

void DynamicArray_destroy_all(void) {
    for (size_t i = 0; i < num_allocated; i++) {
        if (allocated_arrays[i] != NULL && allocated_arrays[i]->buffer != NULL) {
            free(allocated_arrays[i]->buffer);
            allocated_arrays[i]->buffer = NULL;
        }
        free(allocated_arrays[i]);
    }
    num_allocated = 0;
}

int DynamicArray_is_valid(const DynamicArray *arr) {
    if (arr == NULL) return 0;
    return arr->buffer != NULL ? 1 : 0;
}
