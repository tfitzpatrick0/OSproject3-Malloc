/* unit_freelist.c: Unit tests for free list */

#include "malloc/block.h"
#include "malloc/counters.h"
#include "malloc/freelist.h"

#include <assert.h>
#include <limits.h>

/* Externals */

extern Block FreeList;
extern Block *free_list_search_ff(size_t size);
extern Block *free_list_search_bf(size_t size);
extern Block *free_list_search_wf(size_t size);

/* Functions */

int test_00_free_list_search_ff() {
    Block b2 = {.capacity = ALIGN(200), .size = 200, .prev = NULL     , .next = &FreeList };
    Block b1 = {.capacity = ALIGN(300), .size = 300, .prev = NULL     , .next = &b2 };
    Block b0 = {.capacity = ALIGN(100), .size = 100, .prev = &FreeList, .next = &b1 };
    b1.prev = &b0; b2.prev = &b1;
    FreeList.next = &b0; FreeList.prev = &b2;

    assert(free_list_search_ff(1000) == NULL);
    assert(free_list_search_ff(100)  == &b0);
    assert(free_list_search_ff(200)  == &b1);
    assert(free_list_search_ff(300)  == &b1);
    return EXIT_SUCCESS;
}

int test_01_free_list_search_bf() {
    Block b2 = {.capacity = ALIGN(200), .size = 200, .prev = NULL     , .next = &FreeList };
    Block b1 = {.capacity = ALIGN(300), .size = 300, .prev = NULL     , .next = &b2 };
    Block b0 = {.capacity = ALIGN(100), .size = 100, .prev = &FreeList, .next = &b1 };
    b1.prev = &b0; b2.prev = &b1;
    FreeList.next = &b0; FreeList.prev = &b2;

    assert(free_list_search_bf(1000) == NULL);
    assert(free_list_search_bf(100)  == &b0);
    assert(free_list_search_bf(200)  == &b2);
    assert(free_list_search_bf(300)  == &b1);
    return EXIT_SUCCESS;
}

int test_02_free_list_search_wf() {
    Block b2 = {.capacity = ALIGN(200), .size = 200, .prev = NULL     , .next = &FreeList };
    Block b1 = {.capacity = ALIGN(300), .size = 300, .prev = NULL     , .next = &b2 };
    Block b0 = {.capacity = ALIGN(100), .size = 100, .prev = &FreeList, .next = &b1 };
    b1.prev = &b0; b2.prev = &b1;
    FreeList.next = &b0; FreeList.prev = &b2;

    assert(free_list_search_wf(1000) == NULL);
    assert(free_list_search_wf(100)  == &b1);
    assert(free_list_search_wf(200)  == &b1);
    assert(free_list_search_wf(300)  == &b1);
    return EXIT_SUCCESS;
}

int test_03_free_list_insert() {
    Block *b0 = block_allocate(100);
    assert(b0);
    free_list_insert(b0);
    assert(FreeList.prev == b0);
    assert(FreeList.next == b0);
    assert(b0->prev == &FreeList);
    assert(b0->next == &FreeList);

    Block *b1 = block_allocate(100);
    assert(b1);
    free_list_insert(b1);
    assert(FreeList.prev == b0);
    assert(FreeList.next == b0);
    assert(b0->prev == &FreeList);
    assert(b0->next == &FreeList);
    assert(Counters[MERGES] == 1);
    assert(Counters[BLOCKS] == 1);
    assert(b0->capacity == ALIGN(100) + sizeof(Block) + ALIGN(100));

    return EXIT_SUCCESS;
}

int test_04_free_list_length() {
    assert(free_list_length() == 0);

    Block b2 = {.prev = NULL     , .next = &FreeList };
    Block b1 = {.prev = NULL     , .next = &b2 };
    Block b0 = {.prev = &FreeList, .next = &b1 };
    b1.prev = &b0; b2.prev = &b1;
    FreeList.next = &b0; FreeList.prev = &b2;

    assert(free_list_length() == 3);

    return EXIT_SUCCESS;
}

/* Main execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0. Test free_list_search_ff\n");
        fprintf(stderr, "    1. Test free_list_search_bf\n");
        fprintf(stderr, "    2. Test free_list_search_wf\n");
        fprintf(stderr, "    3. Test free_list_insert\n");
        fprintf(stderr, "    4. Test free_list_length\n");
        return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_free_list_search_ff(); break;
        case 1:  status = test_01_free_list_search_bf(); break;
        case 2:  status = test_02_free_list_search_wf(); break;
        case 3:  status = test_03_free_list_insert(); break;
        case 4:  status = test_04_free_list_length(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }

    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
