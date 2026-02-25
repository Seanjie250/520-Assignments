#include "dynamic_array.h"
#include <stdio.h>

int is_positive(double x) {
    return x > 0;
}

int main() {
    printf("=== DynamicArray Demo ===\n\n");
    
    DynamicArray *arr = DynamicArray_new(10);
    DynamicArray_append(arr, 5.0);
    DynamicArray_append(arr, 2.0);
    DynamicArray_append(arr, 8.0);
    DynamicArray_append(arr, 1.0);
    DynamicArray_append(arr, 9.0);
    
    printf("Array: [5.0, 2.0, 8.0, 1.0, 9.0]\n");
    printf("Min: %.1f\n", DynamicArray_min(arr));
    printf("Max: %.1f\n", DynamicArray_max(arr));
    printf("Mean: %.1f\n", DynamicArray_mean(arr));
    printf("Median: %.1f\n", DynamicArray_median(arr));
    printf("Sum: %.1f\n", DynamicArray_sum(arr));
    printf("First: %.1f\n", DynamicArray_first(arr));
    printf("Last: %.1f\n", DynamicArray_last(arr));
    
    DynamicArray *range_arr = DynamicArray_range(0, 1, 0.1);
    printf("\nRange(0, 1, 0.1) size: %zu\n", DynamicArray_size(range_arr));
    
    DynamicArray *filtered = DynamicArray_filter(arr, is_positive);
    printf("Filtered (positive) size: %zu\n", DynamicArray_size(filtered));
    
    DynamicArray_destroy(arr);
    DynamicArray_destroy(range_arr);
    DynamicArray_destroy(filtered);
    
    printf("\n=== Demo Complete ===\n");
    return 0;
}
