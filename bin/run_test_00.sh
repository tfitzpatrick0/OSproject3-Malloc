#!/bin/bash

# Functions

test-library() {
    library=$1
    printf "  Testing %-30s ... " $library
    if diff -y <(env LD_PRELOAD=./lib/$library ./bin/test_00 2> /dev/null) <(test-output) >& test.log; then
    	echo "Success"
    else
    	echo "Failure"
    	cat test.log
    	echo ""
    fi
}

test-output() {
    cat <<EOF
blocks:      0
free blocks: 0
mallocs:     10
frees:       10
callocs:     0
reallocs:    0
reuses:      0
grows:       10
shrinks:     10
splits:      0
merges:      0
requested:   10240
heap size:   0
internal:    0.00
external:    0.00
EOF
}

# Main execution

trap "rm -f test.log" EXIT INT

test-library libmalloc-ff.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so

# vim: sts=4 sw=4 ts=8 ft=sh
