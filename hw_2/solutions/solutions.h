#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <stdlib.h>

// Exercise 1: Running total
int running_total(int n);

// Exercise 2: Reverse array (returns new array)
int* reverse(int* arr, int length);

// Exercise 3: Reverse array in place
void reverse_in_place(int* arr, int length);

// Exercise 4: Count occurrences
int num_occurences(int* arr, int length, int value);

// Exercise 5: Remove duplicates
int* remove_duplicates(int* arr, int length, int* new_length);

// Exercise 6: Reverse string
char* string_reverse(const char* str);

// Exercise 7: Transpose matrix
int* transpose(int* matrix, int rows, int cols);

// Exercise 8: Split string
char** split_string(const char* str, char delimiter, int* count);

// Helper function to free string array
void free_string_array(char** arr, int count);

#endif // SOLUTIONS_H

