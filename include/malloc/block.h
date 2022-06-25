/* block.h: Block Structure */

#ifndef BLOCK_H
#define BLOCK_H

#include "malloc/block.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Block Constants */

#define ALIGNMENT       (sizeof(double))
#define ALIGN(size)     (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SBRK_FAILURE    ((void *)(-1))
#define TRIM_THRESHOLD  (1<<10)

/* Block Structure */

typedef struct block Block;
struct block {
    size_t   capacity;	/* Number of bytes allocated to block (aligned) */
    size_t   size;	/* Number of bytes used by block */
    Block *  prev;	/* Pointer to previous block structure */
    Block *  next;	/* Pointer to next block structure */
    char     data[];	/* Label for user accessible block data */
};

/* Block Macros */

#define BLOCK_FROM_POINTER(ptr) \
    (Block *)((intptr_t)(ptr) - sizeof(Block))

/* Block Functions */

Block * block_allocate(size_t size);
bool    block_release(Block *block);

Block * block_detach(Block *block);

bool    block_merge(Block *dst, Block *src);
Block * block_split(Block *block, size_t size);

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
