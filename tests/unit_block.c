/* unit_block.c: Unit tests for block structures */

#include "malloc/block.h"
#include "malloc/counters.h"

#include <assert.h>
#include <limits.h>

/* Functions */

int test_00_block_allocate() {
    size_t s0 = 100;
    Block *b0 = block_allocate(s0);

    assert(b0);
    assert(b0->size == s0);
    assert(b0->prev == b0);
    assert(b0->next == b0);
    assert(Counters[HEAP_SIZE] == ALIGN(sizeof(Block) + s0));
    assert(Counters[BLOCKS] == 1);
    assert(Counters[GROWS] == 1);

    Block *b1 = block_allocate(LONG_MAX);
    assert(b1 == NULL);
    assert(Counters[HEAP_SIZE] == ALIGN(sizeof(Block) + s0));
    assert(Counters[BLOCKS] == 1);
    assert(Counters[GROWS] == 1);
    return EXIT_SUCCESS;
}

int test_01_block_release() {
    size_t s0 = 100;
    Block *b0 = block_allocate(s0);
    assert(b0);
    assert(block_release(b0) == false);
    assert(Counters[BLOCKS] == 1);
    assert(Counters[GROWS] == 1);
    assert(Counters[SHRINKS] == 0);
    assert(Counters[HEAP_SIZE] == ALIGN(sizeof(Block) + s0));

    size_t s1 = TRIM_THRESHOLD;
    Block *b1 = block_allocate(s1);
    assert(b1);
    assert(block_release(b1) == true);
    assert(Counters[BLOCKS] == 1);
    assert(Counters[GROWS] == 2);
    assert(Counters[SHRINKS] == 1);
    assert(Counters[HEAP_SIZE] == ALIGN(sizeof(Block) + s0));

    return EXIT_SUCCESS;
}

int test_02_block_detach() {
    Block b2 = {.prev = NULL, .next = NULL};
    Block b1 = {.prev = NULL, .next = &b2 };
    Block b0 = {.prev = &b2 , .next = &b1 };
    b1.prev = &b0; b2.prev = &b1; b2.next = &b0;

    assert(block_detach(&b1) == &b1);
    assert(b1.next == &b1);
    assert(b1.prev == &b1);
    assert(b0.next == &b2);
    assert(b2.prev == &b0);

    assert(block_detach(&b0) == &b0);
    assert(b0.next == &b0);
    assert(b0.prev == &b0);
    assert(b2.prev == &b2);
    assert(b2.next == &b2);

    return EXIT_SUCCESS;
}

int test_03_block_merge() {
    size_t s0 = 100;
    Block *b0 = block_allocate(s0);
    assert(b0);

    size_t s1 = 100;
    Block *b1 = block_allocate(s1);
    assert(b1);

    assert(block_merge(b1, b0) == false);
    assert(Counters[MERGES] == 0);
    assert(Counters[BLOCKS] == 2);

    assert(block_merge(b0, b1) == true);
    assert(Counters[MERGES] == 1);
    assert(Counters[BLOCKS] == 1);
    return EXIT_SUCCESS;
}

int test_04_block_split() {
    size_t s0 = 100;
    Block *b0 = block_allocate(s0);
    assert(b0);

    assert(block_split(b0, s0) == b0);
    assert(Counters[SPLITS] == 0);
    assert(Counters[BLOCKS] == 1);

    size_t s1 = 50;
    assert(block_split(b0, s1) != NULL);
    assert(Counters[SPLITS] == 1);
    assert(Counters[BLOCKS] == 2);
    assert(b0->next->capacity == (ALIGN(s0) - ALIGN(s1) - sizeof(Block)));
    assert(b0->next->prev == b0);
    return EXIT_SUCCESS;
}

/* Main execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0. Test block_allocate\n");
        fprintf(stderr, "    1. Test block_release\n");
        fprintf(stderr, "    2. Test block_detach\n");
        fprintf(stderr, "    3. Test block_merge\n");
        fprintf(stderr, "    4. Test block_split\n");
        return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_block_allocate(); break;
        case 1:  status = test_01_block_release(); break;
        case 2:  status = test_02_block_detach(); break;
        case 3:  status = test_03_block_merge(); break;
        case 4:  status = test_04_block_split(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }

    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
