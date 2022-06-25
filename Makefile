CC=       	gcc
CFLAGS= 	-g -std=gnu99 -Wall -Iinclude
LDFLAGS=
LIBRARIES=      lib/libmalloc-ff.so \
		lib/libmalloc-bf.so \
		lib/libmalloc-wf.so
HEADERS=	$(wildcard include/malloc/*.h)
SOURCES=	$(wildcard src/*.c)
TESTS=		$(patsubst tests/%,bin/%,$(patsubst %.c,%,$(wildcard tests/*.c)))

all:    $(LIBRARIES) $(TESTS)

lib/libmalloc-ff.so:   	$(SOURCES) $(HEADERS)
	@echo "Building $@"
	@$(CC) -shared -fPIC $(CFLAGS) -DFIT=0 -o $@ $(SOURCES) $(LDFLAGS)

lib/libmalloc-wf.so:   	$(SOURCES) $(HEADERS)
	@echo "Building $@"
	@$(CC) -shared -fPIC $(CFLAGS) -DFIT=1 -o $@ $(SOURCES) $(LDFLAGS)

lib/libmalloc-bf.so:   	$(SOURCES) $(HEADERS)
	@echo "Building $@"
	@$(CC) -shared -fPIC $(CFLAGS) -DFIT=2 -o $@ $(SOURCES) $(LDFLAGS)

bin/test_%:		tests/test_%.c
	@echo "Building $@"
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bin/unit_%:		tests/unit_%.c src/counters.c src/block.c src/freelist.c
	@echo "Building $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test-units:		$(TESTS)
	@for test in bin/run_*_unit.sh; do 	\
	    $$test;				\
	done

test-applications: 	$(TESTS) $(LIBRARIES)
	@for test in bin/run_test_*.sh; do 	\
	    echo "Running $$(basename $$test)";	\
	    $$test;				\
	    echo "";				\
	done

test-all:		test-units test-applications

test:
	@$(MAKE) -sk test-all

clean:
	@echo "Removing libraries"
	@rm -f $(LIBRARIES)
	@echo "Removing tests"
	@rm -f $(TESTS) test.log

.PHONY: all clean
