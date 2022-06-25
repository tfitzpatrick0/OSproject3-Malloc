/* test_04.c: example from the slides */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */

#define N    (1<<15)

/* Main Execution */

int main(int argc, char *argv[]) {
    char * p0 = malloc(32);
    char * pa = malloc(1);
    char * p1 = malloc(64);
    char * pb = malloc(1);
    char * p2 = malloc(16);

    free(p0);
    free(p1);
    free(p2);

    char * pc = malloc(12);

    if (strstr(argv[1], "ff")) {
    	assert(pc == p0);
    } else if (strstr(argv[1], "bf")) {
    	assert(pc == p2);
    } else if (strstr(argv[1], "wf")) {
    	assert(pc == p1);
    }

    free(pa);
    free(pb);
    free(pc);

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
