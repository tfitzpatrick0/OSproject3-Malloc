/* counters.c: Counters */

#include "malloc/block.h"
#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Global Variables */

extern Block FreeList;
size_t Counters[NCOUNTERS] = {0};
int    DumpFD              = -1;

/* Functions */

/**
 * Initialize counters by doing the following:
 *
 *  1. Register the dump_counters function to run when the program terminates.
 *  2. Duplicate standard output file descriptor to the DumpFD global variable.
 *
 * Note, these actions should only be performed once regardless of how many
 * times the function is called.
 **/
void init_counters() {
    static bool initialized = false;

    if (!initialized) {
        assert(atexit(dump_counters) == 0);
        initialized = true;
        DumpFD      = dup(STDOUT_FILENO);
        assert(DumpFD >= 0);
    }
}

/**
 * Compute internal fragmentation in heap using the formula:
 *
 *  FRAGMENTATION = Sum(internal fragments) / HeapSize * 100.0
 *
 * @return  Percentage of internal fragmentation in heap.
 **/
double  internal_fragmentation() {

    // TODO: Implement internal fragmentation computation
    double internal_fragments = 0;

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {
        internal_fragments += (curr->capacity - curr->size);
    }

    // Avoid dividing by 0
    if (Counters[HEAP_SIZE] == 0) {
        return 0;
    }

    double internal_fragmentation = (internal_fragments / Counters[HEAP_SIZE]) * 100.0;
    return internal_fragmentation;
}

/**
 * Compute external fragmentation in heap using the formula:
 *
 *  FRAGMENTATION = (1 - (LARGEST_FREE_BLOCK / ALL_FREE_MEMORY)) * 100.0
 *
 * https://www.edn.com/design/systems-design/4333346/Handling-memory-fragmentation
 *
 * @return  Percentage of external fragmentation in heap.
 **/
double  external_fragmentation() {

    // TODO: Implement external fragmentation computation
    double all_free_mem = 0;
    double largest_free_block = 0;

    for (Block *curr = FreeList.next; curr != &FreeList; curr = curr->next) {

        all_free_mem += (double)curr->capacity;

        if (curr->capacity > largest_free_block) {
            largest_free_block = (double)curr->capacity;
        }
    }

    if (all_free_mem == 0) {
        return 0;
    }

    double externalFragmentation = (1 - (largest_free_block / all_free_mem)) * 100.0;
    return externalFragmentation;
}

/**
 * Display all counters to the DumpFD global file descriptor saved in
 * init_counters.
 *
 * Note, the function should close the DumpFD global file descriptor at the end
 * of the function.
 **/
void dump_counters() {
    char buffer[BUFSIZ];
    assert(DumpFD >= 0);

    fdprintf(DumpFD, buffer, "blocks:      %lu\n"   , Counters[BLOCKS]);
    fdprintf(DumpFD, buffer, "free blocks: %lu\n"   , free_list_length());
    fdprintf(DumpFD, buffer, "mallocs:     %lu\n"   , Counters[MALLOCS]);
    fdprintf(DumpFD, buffer, "frees:       %lu\n"   , Counters[FREES]);
    fdprintf(DumpFD, buffer, "callocs:     %lu\n"   , Counters[CALLOCS]);
    fdprintf(DumpFD, buffer, "reallocs:    %lu\n"   , Counters[REALLOCS]);
    fdprintf(DumpFD, buffer, "reuses:      %lu\n"   , Counters[REUSES]);
    fdprintf(DumpFD, buffer, "grows:       %lu\n"   , Counters[GROWS]);
    fdprintf(DumpFD, buffer, "shrinks:     %lu\n"   , Counters[SHRINKS]);
    fdprintf(DumpFD, buffer, "splits:      %lu\n"   , Counters[SPLITS]);
    fdprintf(DumpFD, buffer, "merges:      %lu\n"   , Counters[MERGES]);
    fdprintf(DumpFD, buffer, "requested:   %lu\n"   , Counters[REQUESTED]);
    fdprintf(DumpFD, buffer, "heap size:   %lu\n"   , Counters[HEAP_SIZE]);
    fdprintf(DumpFD, buffer, "internal:    %4.2lf\n", internal_fragmentation());
    fdprintf(DumpFD, buffer, "external:    %4.2lf\n", external_fragmentation());

    close(DumpFD);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */