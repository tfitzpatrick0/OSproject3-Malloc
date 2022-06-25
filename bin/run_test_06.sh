#!/bin/bash

# Functions

test-library() {
    library=$1
    shift
    command=$@
    echo -n "  Testing $library ($command)... "
    if env LD_PRELOAD=./lib/$library $command > /dev/null 2>&1; then
    	echo success
    else
    	echo failure
    fi
}

test-libraries() {
    fits="ff bf wf"
    for fit in $fits; do
    	test-library libmalloc-$fit.so $@
    done
}

# Main execution

test-libraries cat tests/*.c
test-libraries md5sum bin/*.sh
test-libraries sha1sum bin/*.sh
test-libraries sort src/*.c
test-libraries dd if=/dev/urandom of=/dev/null bs=1024 count=1024
test-libraries du /lib/
test-libraries find /lib/

# vim: sts=4 sw=4 ts=8 ft=sh
