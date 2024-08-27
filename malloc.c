#include <pthread.h>
#include <unistd.h>
#include "malloc.h"

pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;
mblock_t *head = NULL, *tail = NULL;

void *my_malloc(size_t size) {
    size_t total_size;
    void *requested_block;
    mblock_t *new_block;

    if (!size)
        return NULL;

    pthread_mutex_lock(&global_malloc_lock);

    total_size = sizeof(mblock_t) + size;
    requested_block = sbrk(total_size);

    if (requested_block == (void *) - 1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    new_block = (mblock_t *)requested_block;
    new_block -> metadata.size = size;
    new_block -> metadata.is_free = 0;
    new_block -> metadata.next = NULL;

    if (!head)
        head = new_block;

    if (tail)
        tail -> metadata.next = new_block;

    tail = new_block;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)((char *)new_block + sizeof(mblock_t));
}

mblock_t *get_free_block(size_t size) {
    mblock_t *current = head;
    while (current) {
        if (current -> metadata.is_free && current -> metadata.size >= size)
            return current;
        current = current -> metadata.next;
    }
    return NULL;
}

