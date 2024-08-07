#include <pthread.h>
#include "malloc.h"

pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;
mblock_t *head = NULL, *tail = NULL;

void *malloc(size_t size) {
    size_t total_size;
    void *requested_block;
    mblock_t *free_block;

    if (!size)
        return NULL;

    pthread_mutex_lock(&global_malloc_lock);
    free_block = get_free_block(size);

    if (free_block) {
        free_block->block.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void *)(free_block + 1);
    }

    total_size = sizeof(mblock_t) + size;
    requested_block = sbrk(total_size);

    if (requested_block == (void *)-1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    free_block = (mblock_t *)requested_block;
    free_block->block.size = size;
    free_block->block.is_free = 0;
    free_block->block.next = NULL;

    if (!head)
        head = free_block;

    if (tail)
        tail->block.next = free_block;

    tail = free_block;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)(free_block + 1);
}

mblock_t *get_free_block(size_t size) {
    mblock_t *current = head;
    while (current) { // first fit approach
        if (current->block.is_free && current->block.size >= size)
            return current;
        current = current->block.next;
    }
    return NULL;
}

