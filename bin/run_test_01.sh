#!/bin/bash

# Functions

test-library() {
    library=$1
    printf "  Testing %-30s ... " $library
    if diff -y <(env LD_PRELOAD=./lib/$library ./bin/test_01 2> /dev/null) <(test-output) >& test.log; then
    	echo "Success"
    else
    	echo "Failure"
    	cat test.log
    	echo ""
    fi
}

test-output() {
    cat <<EOF
blocks:      1
free blocks: 1
mallocs:     11
frees:       11
callocs:     0
reallocs:    0
reuses:      9
grows:       2
shrinks:     1
splits:      9
merges:      9
requested:   2047
heap size:   544
internal:    93.93
external:    0.00
EOF
}

# Main execution

trap "rm -f test.log" EXIT INT

test-library libmalloc-ff.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so

# vim: sts=4 sw=4 ts=8 ft=sh
