/* block.c: Block Structure */

#include "malloc/block.h"
#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern Block FreeList;

/**
 * Allocate a new block on the heap using sbrk:
 *
 *  1. Determined aligned amount of memory to allocate.
 *  2. Allocate memory on the heap.
 *  3. Set allocage block properties.
 *
 * @param   size    Number of bytes to allocate.
 * @return  Pointer to data portion of newly allocate block.
 **/
Block * block_allocate(size_t size) {

    // Allocate block
    intptr_t allocated = sizeof(Block) + ALIGN(size);
    Block *  block     = sbrk(allocated);

    if (block == SBRK_FAILURE) {
        return NULL;
    }

    // Record block information
    block->capacity = ALIGN(size);
    block->size     = size;
    block->prev     = block;
    block->next     = block;

    // Update counters
    Counters[HEAP_SIZE] += allocated;
    Counters[BLOCKS]++;
    Counters[GROWS]++;
    return block;
}

/**
 * Attempt to release memory used by block to heap:
 *
 *  1. If the block is at the end of the heap.
 *  2. The block capacity meets the trim threshold.
 *
 * @param   block   Pointer to block to release.
 * @return  Whether or not the release completed successfully.
 **/
bool block_release(Block *block) {

    // End of heap
    void *heap_end = sbrk(0);
    if (heap_end == SBRK_FAILURE) {
        return false;
    }

    // TODO: Implement block release
    void *block_end = (void *)(block->capacity + block->data);
    if (((block->capacity + sizeof(Block)) >= TRIM_THRESHOLD) && (heap_end == block_end)) {
        // Calculate allocated memory and decrement heap with sbrk
        size_t allocated = block->capacity + sizeof(Block);
        block = block_detach(block);

        void *sbrk_stat = sbrk(allocated * (-1));
        if (sbrk_stat == SBRK_FAILURE) {
            return false;
        }

        Counters[BLOCKS]--;
        Counters[SHRINKS]++;
        Counters[HEAP_SIZE] -= allocated;
        return true;
    }

    return false;
}

/**
 * Detach specified block from its neighbors.
 *
 * @param   block   Pointer to block to detach.
 * @return  Pointer to detached block.
 **/
Block * block_detach(Block *block) {

    // TODO: Detach block from neighbors by updating previous and next block
    if (block) {
        Block *prev = block->prev;
        Block *next = block->next;

        prev->next  = block->next;
        next->prev  = block->prev;

        block->next = block;
        block->prev = block;
    }

    return block;
}

/**
 * Attempt to merge source block into destination.
 *
 *  1. Compute end of destination and start of source.
 *
 *  2. If they both match, then merge source into destination by giving the
 *  destination all of the memory allocated to source.
 *
 *  3. If destination is not already in the list, insert merged destination
 *  block into list by updating appropriate references.
 *
 * @param   dst     Destination block we are merging into.
 * @param   src     Source block we are merging from.
 * @return  Whether or not the merge completed successfully.
 **/
bool block_merge(Block *dst, Block *src) {

    // TODO: Implement block merge
    // Compute end of destination
    intptr_t dst_end = (intptr_t)dst + dst->capacity + sizeof(Block);

    // Check if destination = source
    if (dst_end == (intptr_t)src) {

        // Merge source capacity into the destination block
        dst->capacity += src->capacity + sizeof(Block);

        // Insert into the list if not there already
        if (dst->next == dst) {
            Block *prev = src->prev;
            Block *next = src->next;

            prev->next = dst;
            next->prev = dst;

            dst->next = next;
            dst->prev = prev;
        }

        Counters[MERGES]++;
        Counters[BLOCKS]--;
        return true;
    }

    return false;
}

/**
 * Attempt to split block with the specified size:
 *
 *  1. Check if block capacity is sufficient for requested aligned size and
 *  header Block.
 *
 *  2. Split specified block into two blocks.
 *
 * @param   block   Pointer to block to split into two separate blocks.
 * @param   size    Desired size of the first block after split.
 * @return  Pointer to original block (regardless if it was split or not).
 **/
Block * block_split(Block *block, size_t size) {

    // TODO: Implement block split
    // Calculate how much space is needed to split
    size_t split_size = sizeof(Block) + ALIGN(size);

    // Check that block capacity is sufficient to split
    if (block && (block->capacity > split_size)) {

        Block *new_block = (Block *)(block->data + ALIGN(size));

        // update block attributes
        new_block->size = block->size - split_size;
        new_block->capacity = block->capacity - split_size;
        
        block->capacity = ALIGN(size);
        block->size = size;

        // update block pointers
        new_block->next = block->next;
        block->next->prev = new_block;

        block->next = new_block;
        new_block->prev = block;

        Counters[SPLITS]++;
        Counters[BLOCKS]++;
    }
    else {
        block->size = size;
    }

    return block;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */