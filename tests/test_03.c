/* test_03.c: allocate chunks of different sizes */

#include <stdio.h>
#include <stdlib.h>

/* Constants */

#define N    10

#define ALIGNMENT       (sizeof(double))
#define ALIGN(size)     (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct block Block;
struct block {
    size_t   capacity;	/* Number of bytes allocated to block (aligned) */
    size_t   size;	/* Number of bytes used by block */
    Block *  prev;	/* Pointer to previous block structure */
    Block *  next;	/* Pointer to next block structure */
    char     data[];	/* Label for user accessible block data */
};

/* Main Execution */

int main(int argc, char *argv[]) {
    char *p[N];

    for (int i = 0; i < N; i++) {
        size_t s = 1<<(N - i);
        fprintf(stderr, "p[%d] = malloc(%lu)\n", i, s);
    	p[i] = malloc(s);
    }
    
    for (int i = 0; i < N; i+=2) {
        fprintf(stderr, "free(%p)\n", p[i]);
    	free(p[i]);
    }
    
    
    for (int i = 0; i < N; i++) {
        size_t s = 1<<i;
        fprintf(stderr, "p[%d] = malloc(%lu)\n", i, s);
    	p[i] = malloc(s);
    }
    
    for (int i = 1; i < N; i+=2) {
        fprintf(stderr, "free(%p)\n", p[i]);
    	free(p[i]);
    }
    
    for (int i = 0; i < N; i++) {
        size_t s = 1<<(N - i);
        fprintf(stderr, "p[%d] = malloc(%lu)\n", i, s);
    	p[i] = malloc(s);
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
