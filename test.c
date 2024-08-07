#include <stdio.h>
#include "malloc.h"

int main() {
    int *ptr = (int *) my_malloc(sizeof(int));
    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    *ptr = 42;
    printf("Value: %d\n", *ptr);

    int *arr_ptr = (int *) my_malloc(10 * sizeof(int));
    if (arr_ptr == NULL) {
        printf("Memory allocation for int array failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        arr_ptr[i] = i * 2;
    }

	printf("Array Values: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr_ptr[i]);
    }
    
	printf("\n");

    // Free the allocated memory (if you have implemented free)
    // free(ptr);
    // free(arr_ptr);

    return 0;
}
