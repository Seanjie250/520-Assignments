#include "solutions.h"
#include <string.h>
#include <stdio.h>

// Simple test framework macros (if not using Unity)
#ifndef TEST
#define TEST(suite, name) void test_##suite##_##name()
#define ASSERT_EQ(actual, expected) \
    do { \
        int _actual = (actual); \
        int _expected = (expected); \
        if (_actual != _expected) { \
            printf("FAIL: Expected %d, got %d\n", _expected, _actual); \
        } else { \
            printf("PASS: %d == %d\n", _expected, _actual); \
        } \
    } while(0)
#define ASSERT_STREQ(actual, expected) \
    do { \
        if (strcmp((actual), (expected)) != 0) { \
            printf("FAIL: Expected '%s', got '%s'\n", (expected), (actual)); \
        } else { \
            printf("PASS: '%s' == '%s'\n", (expected), (actual)); \
        } \
    } while(0)
#endif

TEST(HW2, RunningTotal) {
    // Reset static variable by getting current total and subtracting it
    // This is a workaround since there's no reset function
    extern int running_total(int n);  // Ensure we can call it
    int current_total = running_total(0);  // Get current total (adds 0, returns current)
    if (current_total != 0) {
        running_total(-current_total);  // Reset to 0
    }
    
    ASSERT_EQ(running_total(1), 1);
    ASSERT_EQ(running_total(1), 2);
    ASSERT_EQ(running_total(5), 7);
    ASSERT_EQ(running_total(-3), 4);
}

TEST(HW2, Reverse) {
    int x[] = {10, 20, 30, 40, 50};
    int* y = reverse(x, 5);
    ASSERT_EQ(y[0], 50);
    ASSERT_EQ(y[1], 40);
    ASSERT_EQ(y[2], 30);
    ASSERT_EQ(y[3], 20);
    ASSERT_EQ(y[4], 10);
    free(y);
}

TEST(HW2, ReverseInPlace) {
    int x[] = {10, 20, 30, 40, 50};
    reverse_in_place(x, 5);
    ASSERT_EQ(x[0], 50);
    ASSERT_EQ(x[1], 40);
    ASSERT_EQ(x[2], 30);
    ASSERT_EQ(x[3], 20);
    ASSERT_EQ(x[4], 10);
}

TEST(HW2, NumInstances) {
    int a[] = {1, 1, 2, 3, 1, 4, 5, 2, 20, 5};
    ASSERT_EQ(num_occurences(a, 10, 1), 3);
}

TEST(HW2, RemoveDuplicates) {
    int a[] = {1, 2, 2, 3, 1, 4, 5, 2};
    int new_len;
    int* result = remove_duplicates(a, 8, &new_len);
    ASSERT_EQ(new_len, 5);
    ASSERT_EQ(result[0], 1);
    ASSERT_EQ(result[1], 2);
    ASSERT_EQ(result[2], 3);
    ASSERT_EQ(result[3], 4);
    ASSERT_EQ(result[4], 5);
    free(result);
}

TEST(HW2, StringReverse) {
    char* result = string_reverse("hello");
    ASSERT_STREQ(result, "olleh");
    free(result);
}

TEST(HW2, Transpose) {
    // 2x3 matrix: [[1,2,3], [4,5,6]]
    int matrix[] = {1, 2, 3, 4, 5, 6};
    int* result = transpose(matrix, 2, 3);
    // Expected 3x2: [[1,4], [2,5], [3,6]]
    int expected[] = {1, 4, 2, 5, 3, 6};
    for (int i = 0; i < 6; i++) {
        ASSERT_EQ(result[i], expected[i]);
    }
    free(result);
}

TEST(HW2, SplitString) {
    int count;
    char** result = split_string("apple,banana,cherry", ',', &count);
    ASSERT_EQ(count, 3);
    ASSERT_STREQ(result[0], "apple");
    ASSERT_STREQ(result[1], "banana");
    ASSERT_STREQ(result[2], "cherry");
    free_string_array(result, count);
}

// Main function to run all tests
int main() {
    printf("Running HW2 Tests...\n\n");
    
    printf("Test: RunningTotal\n");
    test_HW2_RunningTotal();
    printf("\n");
    
    printf("Test: Reverse\n");
    test_HW2_Reverse();
    printf("\n");
    
    printf("Test: ReverseInPlace\n");
    test_HW2_ReverseInPlace();
    printf("\n");
    
    printf("Test: NumInstances\n");
    test_HW2_NumInstances();
    printf("\n");
    
    printf("Test: RemoveDuplicates\n");
    test_HW2_RemoveDuplicates();
    printf("\n");
    
    printf("Test: StringReverse\n");
    test_HW2_StringReverse();
    printf("\n");
    
    printf("Test: Transpose\n");
    test_HW2_Transpose();
    printf("\n");
    
    printf("Test: SplitString\n");
    test_HW2_SplitString();
    printf("\n");
    
    printf("All tests completed!\n");
    return 0;
}

