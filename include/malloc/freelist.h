/* freelist.h: Free List Structure */

#ifndef FREELIST_H
#define FREELIST_H

#include "malloc/block.h"

/* Free List Functions */

Block *	free_list_search(size_t size);
void	free_list_insert(Block *block);
size_t  free_list_length();

#endif

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
