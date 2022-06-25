/* posix.c: POSIX API Implementation */

#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <assert.h>
#include <string.h>

/**
 * Allocate specified amount memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to the requested amount of memory.
 **/
void *malloc(size_t size) {

    // Initialize counters
    init_counters();

    // Handle empty size
    if (!size) {
        return NULL;
    }

    // Search free list for any available block with matching size
    Block *block = free_list_search(size);

    // block found in free list
    if (block) {
        block = block_split(block, size);
        block = block_detach(block);
    }
    // block not found in free list, allocate block
    else {
        block = block_allocate(size);
    }

    // Could not find free block or allocate a block, so just return NULL
    if (!block) {
        return NULL;
    }

    // Update counters
    Counters[MALLOCS]++;
    Counters[REQUESTED] += size;

    // Return data address associated with block
    return block->data;
}

/**
 * Release previously allocated memory.
 * @param   ptr     Pointer to previously allocated memory.
 **/
void free(void *ptr) {

    if (!ptr) {
        return;
    }

    // Update counters
    Counters[FREES]++;

    // TODO: Try to release block, otherwise insert it into the free list
    Block *block = BLOCK_FROM_POINTER(ptr);
    if (!block_release(block)) {
        free_list_insert(block);
    }
}



/**
 * Allocate memory with specified number of elements and with each element set
 * to 0.
 * @param   nmemb   Number of elements.
 * @param   size    Size of each element.
 * @return  Pointer to requested amount of memory.
 **/
void *calloc(size_t nmemb, size_t size) {

    // TODO: Implement calloc
    size_t max_items = SIZE_MAX / size;
    if (nmemb > max_items) {
        return NULL;
    }

    size_t total_size = nmemb * size;
    void *new_ptr = malloc(total_size);
    memset(new_ptr, 0, total_size);

    if (!new_ptr) {
        return NULL;
    }

    Counters[CALLOCS]++;
    return new_ptr;
}


/**
 * Reallocate memory with specified size.
 * @param   ptr     Pointer to previously allocated memory.
 * @param   size    Amount of bytes to allocate.
 * @return  Pointer to requested amount of memory.
 **/
void *realloc(void *ptr, size_t size) {

    // TODO: Implement realloc
    if (!ptr) {
        // NULL ptr, use malloc
        return malloc(size);
    }

    Block *block = BLOCK_FROM_POINTER(ptr);
    void *new_ptr = malloc(size);

    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, block->size);
    free(ptr);
    Counters[REALLOCS]++;
    return new_ptr;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */