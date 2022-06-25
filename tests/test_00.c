/* test_00.c: allocate same size block over and over again */

#include <stdio.h>
#include <stdlib.h>

/* Constants */

#define N	10
#define SIZE	(1<<10)

/* Main Execution */

int main(int argc, char *argv[]) {
    for (int i = 0; i < N; i++) {
        fprintf(stderr, "malloc(%d)\n", i);
    	int *p = malloc(SIZE);
        fprintf(stderr, "free(%p)\n", p);
    	free(p);
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
