# Custom Memory Allocation Library

The inspiration for this project was my Operating Systems class at BBU Cluj-Napoca, during which, I got my hands on the Standard Library Memory Allocator. Using the knowledge obtained then, I created my humble version of the memory allocator, which thrives not in efficiency, but in being my very own shot at it. However, a great source of inspiration was [this article][memory-article].

## Implementation Details

- **Concurrency** - in order to avoid race conditions (different processes read and write the same memory blocks simultaneously), I used `pthread_mutex_lock`, that ensures mutual exclusivity for one process to the memory block.
- **Memory Acquisition** - my implementation uses the system call [void *sbrk(int incr)][sbrk-link] to increase or decrease the program's data segment size and therefore, request more memory from the operating system.
- **Memory Representation** - the allocator maintains a singly linked list of all memory blocks, both allocated and free. 

## Functionalities
1. `void *my_malloc(size_t nsize)` - allocates a block of memory of the specified size 
2. `void my_free(void *block)` - frees a previously allocated block of memory
3. `void *my_calloc(size_t num, size_t nsize)` - allocates memory for an array of elements and initializes them to zero
4. `void *my_realloc(void *block, size_t nsize)` - changes the size of a previously allocated memory block

## Building the Project

To build the project, run the `build.sh` script:

```bash
./build.sh
```

## Usage

Include the `malloc.h` header in your C program:

```c
#include "malloc.h"
```

[memory-article]: https://arjunsreedharan.org/post/148675821737/memory-allocators-101-write-a-simple-memory
[sbrk-link]: https://www.ibm.com/docs/sk/zos/2.4.0?topic=functions-sbrk-change-space-allocation
