#!/bin/bash

# Functions

test-library() {
    library=$1
    printf "  Testing %-30s ... " $library
    if diff -y <(env LD_PRELOAD=./lib/$library ./bin/test_04 $library 2> /dev/null) <($library-output) >& test.log; then
    	echo "Success"
    else
    	echo "Failure"
    	cat test.log
    	echo ""
    fi
}

libmalloc-ff.so-output() {
    cat <<EOF
blocks:      2
free blocks: 2
mallocs:     6
frees:       6
callocs:     0
reallocs:    0
reuses:      1
grows:       5
shrinks:     0
splits:      0
merges:      3
requested:   126
heap size:   288
internal:    68.06
external:    7.14
EOF
}

libmalloc-bf.so-output() {
    cat <<EOF
blocks:      2
free blocks: 2
mallocs:     6
frees:       6
callocs:     0
reallocs:    0
reuses:      1
grows:       5
shrinks:     0
splits:      0
merges:      3
requested:   126
heap size:   288
internal:    44.44
external:    32.14
EOF
}

libmalloc-wf.so-output() {
    cat <<EOF
blocks:      3
free blocks: 3
mallocs:     6
frees:       6
callocs:     0
reallocs:    0
reuses:      1
grows:       5
shrinks:     0
splits:      1
merges:      3
requested:   126
heap size:   288
internal:    44.44
external:    37.50
EOF
}

# Main execution

trap "rm -f test.log" EXIT INT

test-library libmalloc-ff.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so

# vim: sts=4 sw=4 ts=8 ft=sh
