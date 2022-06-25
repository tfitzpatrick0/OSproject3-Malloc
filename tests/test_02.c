/* test_02.c: allocate chunks and free middle */

#include <stdio.h>
#include <stdlib.h>

/* Constants */

#define SIZE 1<<10

/* Main Execution */

int main(int argc, char *argv[]) {

    fputs("p0 malloc\n", stderr);
    int *p0 = malloc(SIZE);
    fputs("p1 malloc\n", stderr);
    int *p1 = malloc(SIZE);
    fputs("p2 malloc\n", stderr);
    int *p2 = malloc(SIZE);
    fputs("p3 malloc\n", stderr);
    int *p3 = malloc(SIZE);

    fputs("p1 free\n", stderr);
    free(p1); 

    fputs("p2 free\n", stderr);
    free(p2);

    fputs("p1 malloc\n", stderr);
    p1 = malloc(SIZE);
    fputs("p2 malloc\n", stderr);
    p2 = malloc(SIZE);

    fputs("p0 free\n", stderr);
    free(p0);
    fputs("p1 free\n", stderr);
    free(p1);
    fputs("p2 free\n", stderr);
    free(p2);
    fputs("p3 free\n", stderr);
    free(p3);

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
