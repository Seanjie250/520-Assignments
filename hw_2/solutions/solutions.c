#include "solutions.h"
#include <string.h>
#include <stdio.h>

// Exercise 1: Running total
// Static variable to maintain state across function calls
static int total = 0;

int running_total(int n) {
    total += n;
    return total;
}

// Exercise 2: Reverse array (returns new array)
int* reverse(int* arr, int length) {
    if (arr == NULL || length <= 0) {
        return NULL;
    }
    
    int* reversed = (int*)calloc(length, sizeof(int));
    if (reversed == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < length; i++) {
        reversed[i] = arr[length - 1 - i];
    }
    
    return reversed;
}

// Exercise 3: Reverse array in place
void reverse_in_place(int* arr, int length) {
    if (arr == NULL || length <= 0) {
        return;
    }
    
    for (int i = 0; i < length / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[length - 1 - i];
        arr[length - 1 - i] = temp;
    }
}

// Exercise 4: Count occurrences
int num_occurences(int* arr, int length, int value) {
    if (arr == NULL || length <= 0) {
        return 0;
    }
    
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (arr[i] == value) {
            count++;
        }
    }
    
    return count;
}

// Exercise 5: Remove duplicates
int* remove_duplicates(int* arr, int length, int* new_length) {
    if (arr == NULL || length <= 0) {
        *new_length = 0;
        return NULL;
    }
    
    // Temporary array to track unique elements
    int* temp = (int*)malloc(length * sizeof(int));
    if (temp == NULL) {
        *new_length = 0;
        return NULL;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < length; i++) {
        int found = 0;
        // Check if we've seen this value before
        for (int j = 0; j < unique_count; j++) {
            if (temp[j] == arr[i]) {
                found = 1;
                break;
            }
        }
        
        // If not found, add to unique list
        if (!found) {
            temp[unique_count] = arr[i];
            unique_count++;
        }
    }
    
    // Allocate final array with exact size needed
    int* result = (int*)malloc(unique_count * sizeof(int));
    if (result == NULL) {
        free(temp);
        *new_length = 0;
        return NULL;
    }
    
    // Copy unique elements to result
    for (int i = 0; i < unique_count; i++) {
        result[i] = temp[i];
    }
    
    free(temp);
    *new_length = unique_count;
    return result;
}

// Exercise 6: Reverse string
char* string_reverse(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    
    int len = strlen(str);
    char* reversed = (char*)malloc((len + 1) * sizeof(char));
    if (reversed == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - 1 - i];
    }
    reversed[len] = '\0';
    
    return reversed;
}

// Exercise 7: Transpose matrix
int* transpose(int* matrix, int rows, int cols) {
    if (matrix == NULL || rows <= 0 || cols <= 0) {
        return NULL;
    }
    
    int* transposed = (int*)malloc(rows * cols * sizeof(int));
    if (transposed == NULL) {
        return NULL;
    }
    
    // Transpose: element at [i][j] goes to [j][i]
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j * rows + i] = matrix[i * cols + j];
        }
    }
    
    return transposed;
}

// Exercise 8: Split string
char** split_string(const char* str, char delimiter, int* count) {
    if (str == NULL) {
        *count = 0;
        return NULL;
    }
    
    int len = strlen(str);
    if (len == 0) {
        *count = 0;
        return NULL;
    }
    
    // First pass: count tokens (handling consecutive delimiters)
    int token_count = 0;
    int in_token = 0;
    
    for (int i = 0; i < len; i++) {
        if (str[i] != delimiter) {
            if (!in_token) {
                token_count++;
                in_token = 1;
            }
        } else {
            in_token = 0;
        }
    }
    
    if (token_count == 0) {
        *count = 0;
        return NULL;
    }
    
    // Allocate array of pointers
    char** result = (char**)malloc(token_count * sizeof(char*));
    if (result == NULL) {
        *count = 0;
        return NULL;
    }
    
    // Second pass: extract tokens
    int token_idx = 0;
    int start = -1;
    
    for (int i = 0; i <= len; i++) {
        if (i == len || str[i] == delimiter) {
            if (start != -1) {
                // Found end of token
                int token_len = i - start;
                result[token_idx] = (char*)malloc((token_len + 1) * sizeof(char));
                if (result[token_idx] == NULL) {
                    // Cleanup on error
                    for (int j = 0; j < token_idx; j++) {
                        free(result[j]);
                    }
                    free(result);
                    *count = 0;
                    return NULL;
                }
                
                strncpy(result[token_idx], str + start, token_len);
                result[token_idx][token_len] = '\0';
                token_idx++;
                start = -1;
            }
        } else {
            if (start == -1) {
                start = i;
            }
        }
    }
    
    *count = token_count;
    return result;
}

// Helper function to free string array
void free_string_array(char** arr, int count) {
    if (arr == NULL) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        free(arr[i]);
    }
    free(arr);
}

