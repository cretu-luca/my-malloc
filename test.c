#include <stdio.h>
#include "malloc.h"

int main() {
	
	int *ptr = (int *) malloc(sizeof(int));
	
	*ptr = 42;

	printf("%d", *ptr);
}
