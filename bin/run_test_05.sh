#!/bin/bash

# Functions

time-library() {
    library=$1
    printf "  Timing %-31s ... " $library
    { time env LD_PRELOAD=./lib/$library ./bin/test_05; } |& awk '/real\s+/ { print $2 }'
}

# Main execution

time-library libmalloc-ff.so
time-library libmalloc-bf.so
time-library libmalloc-wf.so

# vim: sts=4 sw=4 ts=8 ft=sh
