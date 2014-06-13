.SUFFIXES:
CC=/usr/bin/gcc
#CFLAGS=-Wall -O1 -Wl,-O1
#OBJ_CFLAGS=-fPIC -fvisibility=hidden -Wl,--relocatable -Wl,--no-undefined
CFLAGS=-Wall
OBJ_CFLAGS=-fPIC -Wl,--relocatable
CP=/bin/cp

SRCDIR=./src
OBJDIR=./obj
LIBDIR=./lib
TSTDIR=./test
BENCHDIR=./bench

#BENCHLIB=benchmark-gettime
BENCHLIB=benchmark-custom

# For file naming
VERSION=$(shell date +%Y%m%d)
DIST=mkhpack-$(VERSION)

# Names of target libraries
NAMES := huffman hpack

# Headers & Objects for using target libraries
huffman_HEADERS = $(LIBDIR)/huffman.h
huffman_OBJECTS = $(OBJDIR)/huffman.o
hpack_HEADERS = $(LIBDIR)/hpack.h
hpack_OBJECTS = $(OBJDIR)/hpack.o

# Headers & Sources for building target libraries
huffman_HEADER = $(SRCDIR)/huffman.h
huffman_SOURCE = $(SRCDIR)/huffman.c
hpack_HEADER = $(SRCDIR)/hpack.h
hpack_SOURCE = $(SRCDIR)/hpack.c

# Lists ...
OBJECTS :=
LIBS :=
HEADERS :=
TEST_OBJECTS :=
TESTS :=
BENCH_OBJECTS :=
BENCHES :=


#
# Default rule
#

.PHONY: all
all: lib


#
# Libraries and tests
#

define LIBRARY_RULES

# Object
ifeq (,$$(findstring $(1).o,$$(OBJECTS)))
	OBJECTS += $(OBJDIR)/$(1).o
	HEADERS += $(LIBDIR)/$(1).h
	LIBS += $(LIBDIR)/$(1).so
endif
$(OBJDIR)/$(1).o: $$($(1)_SOURCE) $$($(1)_HEADER)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@

# Shared object
$(LIBDIR)/$(1).so: $(OBJDIR)/$(1).o
	$$(CC) -shared $$< -o $$@

# Test object
TEST_OBJECTS += $(TSTDIR)/test-$(1).o
$(TSTDIR)/test-$(1).o: $(TSTDIR)/test-$(1).c $$($(1)_HEADERS)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@

# Test program
TESTS += $(TSTDIR)/test-$(1)
$(TSTDIR)/test-$(1): $(TSTDIR)/test-$(1).o $$($(1)_OBJECTS)
	$$(CC) $$(CFLAGS) $$^ -o $$@

# Benchmark object
BENCH_OBJECTS += $(BENCHDIR)/bench-$(1).o
$(BENCHDIR)/bench-$(1).o: $(BENCHDIR)/bench-$(1).c $$($(1)_HEADERS)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@

# Benchmark program
BENCHES += $(BENCHDIR)/bench-$(1)
$(BENCHDIR)/bench-$(1): $(BENCHDIR)/bench-$(1).o $(BENCHDIR)/$(BENCHLIB).o $$($(1)_OBJECTS)
	$$(CC) $$(CFLAGS) -Wl,--no-as-needed -Wl,-lrt $$^ -o $$@

endef
$(foreach lib,$(NAMES),$(eval $(call LIBRARY_RULES,$(lib))))


$(LIBDIR)/%.h: $(SRCDIR)/%.h
	$(CP) $< $@

# Standard benchmark library/object
BENCH_OBJECTS += $(BENCHDIR)/$(BENCHLIB).o
$(BENCHDIR)/$(BENCHLIB).o: $(BENCHDIR)/$(BENCHLIB).c $(BENCHDIR)/benchmark.h
	$(CC) $(OBJ_CFLAGS) $(CFLAGS) -c $< -o $@


#
# The distribution
#

DISTFILE=$(DIST).tar.gz
$(DISTFILE): lib
	tar cvzf $(DISTFILE) --transform='s,$(LIBDIR)/,$(DIST)/,' $(HEADERS) $(LIBS)


#
# Non-standard targets
#

.PHONY: lib tests benchmarks bench

lib: $(HEADERS) $(LIBS)

tests: $(TESTS)

benchmarks: $(BENCHES)

bench: benchmarks
	@$(foreach b,$(BENCHES),echo "$(b)"; ./$(b) 2> /dev/null) echo "Done"

#
# Standard targets
#

.PHONY: dist check clean distclean

dist: $(DISTFILE)

check: tests
	@$(foreach t,$(TESTS),echo "$(t)"; ./$(t) > /dev/null; echo "$$? test failures";) echo "Done"

clean:
	-rm $(OBJECTS) $(LIBS) $(HEADERS) $(TESTS) $(TEST_OBJECTS) $(BENCHES) $(BENCH_OBJECTS)

distclean:
	-rm $(DISTFILE)

