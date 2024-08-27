#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
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
    
    if (total_size < size) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    requested_block = sbrk(total_size);

    if (requested_block == (void *) - 1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    new_block = (mblock_t *)requested_block;
    new_block->metadata.size = size;
    new_block->metadata.is_free = 0;
    new_block->metadata.next = NULL;

    if (!head)
        head = new_block;

    if (tail)
        tail->metadata.next = new_block;

    tail = new_block;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)(new_block + 1);
}

mblock_t *get_free_block(size_t nsize) {
    mblock_t *current = head;
    while (current) {
        if (current -> metadata.is_free && current -> metadata.size >= nsize)
            return current;
        current = current -> metadata.next;
    }
    return NULL;
}

void my_free(void *block) { 
    mblock_t *current, *temp;
    void *programBreak;

    if(!block) {
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    current = (mblock_t *) block - 1;

    programBreak = sbrk(0);
    if((char *) block + current -> metadata.size == programBreak) {
        if(head == tail) {
            head = tail = NULL;
        } else {
            temp = head;
            while(temp) {
                if(temp -> metadata.next == tail) {
                    temp -> metadata.next = NULL;
                    tail = temp;
                }
                temp = temp -> metadata.next;
            }
        }
        sbrk(0 - sizeof(mblock_t) - current -> metadata.size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    current -> metadata.is_free = true;
    pthread_mutex_unlock(&global_malloc_lock);
}

void *my_calloc(size_t num, size_t nsize) {
    size_t total_size;
    void *block;

    if (!num || !nsize)
        return NULL;

    if (num > SIZE_MAX / nsize)
        return NULL;

    total_size = num * nsize;
    
    block = my_malloc(total_size);
    if (!block)
        return NULL;

    memset(block, 0, total_size);
    return block;
}


void *my_realloc(void *block, size_t nsize) {
    mblock_t *current;
    void *ret;
    if (!block || !nsize)
        return my_malloc(nsize);
    current = (mblock_t *)block - 1;
    if (current->metadata.size >= nsize)
        return block;
    ret = my_malloc(nsize);
    if (ret) {
        memcpy(ret, block, current->metadata.size);
        my_free(block);
    }
    return ret;
}