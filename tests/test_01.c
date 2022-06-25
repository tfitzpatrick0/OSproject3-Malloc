/* test_01.c: allocate descending sized blocks */

#include <stdio.h>
#include <stdlib.h>

/* Constants */

#define SIZE	(1<<10)

/* Main Execution */

int main(int argc, char *argv[]) {
    for (size_t s = SIZE; s > 0; s>>=1) {
        fprintf(stderr, "malloc(%lu)\n", s);
    	int *p = malloc(s);
        fprintf(stderr, "free(%p)\n", p);
    	free(p);
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
