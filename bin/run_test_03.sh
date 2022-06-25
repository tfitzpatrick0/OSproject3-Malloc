#!/bin/bash

# Functions

test-library() {
    library=$1
    printf "  Testing %-30s ... " $library
    if diff -y <(env LD_PRELOAD=./lib/$library ./bin/test_03 2> /dev/null) <($library-output) >& test.log; then
    	echo "Success"
    else
    	echo "Failure"
    	cat test.log
    	echo ""
    fi
}

libmalloc-ff.so-output() {
    cat <<EOF
blocks:      22
free blocks: 2
mallocs:     30
frees:       10
callocs:     0
reallocs:    0
reuses:      18
grows:       12
shrinks:     0
splits:      10
merges:      0
requested:   5115
heap size:   3976
internal:    0.00
external:    20.00
EOF
}

libmalloc-bf.so-output() {
    cat <<EOF
blocks:      21
free blocks: 1
mallocs:     30
frees:       10
callocs:     0
reallocs:    0
reuses:      17
grows:       13
shrinks:     0
splits:      9
merges:      1
requested:   5115
heap size:   3816
internal:    0.00
external:    0.00
EOF
}

libmalloc-wf.so-output() {
    cat <<EOF
blocks:      23
free blocks: 3
mallocs:     30
frees:       10
callocs:     0
reallocs:    0
reuses:      18
grows:       12
shrinks:     0
splits:      11
merges:      0
requested:   5115
heap size:   3976
internal:    0.25
external:    66.67
EOF
}

# Main execution

trap "rm -f test.log" EXIT INT

test-library libmalloc-ff.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so

# vim: sts=4 sw=4 ts=8 ft=sh
