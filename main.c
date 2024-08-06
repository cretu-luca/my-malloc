#include "main.h"

pthread_mutex_t global_malloc_lock;

void *malloc(size_t size) {
	size_t total_size;
	void *requested_block;
	mblock_t *free_block;

	if(!size) 
		return NULL;

	pthread_mutex_lock(&global_malloc_lock);
	free_block = get_free_block(size);

	if(free_block) {
		free_block -> block.is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void *)(header + 1);
	}

	total_size = sizeof(free_block) + size;
	requested_block = sbrk(total_size);

	if(requested_block = (void *) -1) {
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}

	free_block = requested_block;
	free_block -> block.size = size;
	free_block -> block.is_free = 0;
	free_block -> block.next = NULL;

	if(!head)
		head = free_block;

	if(tail)
		tail -> block.next = free_block;

	tail = free_block;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void *)(header + 1);
}

mblock_t *get_free_block(size_t size) {
	mblock_t *current = head;
	while(current != NULL) { // first fit approach
		if(current -> mblock.is_free && current -> mblock.size >= size)
			return current;
		current = current -> mblock.next; 
	}

	return NULL;
}
