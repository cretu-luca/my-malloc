#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "malloc.h"

void test_my_malloc_and_my_free() {
    int *ptr = (int *) my_malloc(sizeof(int));
    assert(ptr != NULL);
    *ptr = 42;
    printf("my_malloc test: Allocated int value: %d\n", *ptr);
    my_free(ptr);
    printf("my_free test: Freed the allocated memory\n");
}

void test_my_calloc() {
    int *arr = (int *) my_calloc(5, sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 5; i++) {
        assert(arr[i] == 0);
    }
    printf("my_calloc test: Allocated and zeroed array of 5 ints\n");
    my_free(arr);
}

void test_my_realloc() {
    int *arr = (int *) my_malloc(5 * sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    
    arr = (int *) my_realloc(arr, 10 * sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 5; i++) {
        assert(arr[i] == i);
    }
    printf("my_realloc test: Successfully reallocated and preserved values\n");
    my_free(arr);
}

void test_edge_cases() {
    void *ptr;
    
    ptr = my_malloc(0);
    assert(ptr == NULL);
    printf("Edge case: my_malloc(0) returned NULL\n");
    
    ptr = my_calloc(0, 10);
    assert(ptr == NULL);
    printf("Edge case: my_calloc(0, 10) returned NULL\n");
    
    ptr = my_calloc(10, 0);
    assert(ptr == NULL);
    printf("Edge case: my_calloc(10, 0) returned NULL\n");
    
    ptr = my_malloc(sizeof(int));
    ptr = my_realloc(ptr, 0);
    assert(ptr == NULL);
    printf("Edge case: my_realloc(ptr, 0) returned NULL\n");
    
    ptr = my_realloc(NULL, 10);
    assert(ptr != NULL);
    printf("Edge case: my_realloc(NULL, 10) allocated memory\n");
    my_free(ptr);
}

int main() {
    printf("Starting memory allocation tests\n");
    
    test_my_malloc_and_my_free();
    test_my_calloc();
    test_my_realloc();
    test_edge_cases();
    
    printf("All tests completed successfully\n");
    return 0;
}