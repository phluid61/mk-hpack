include Makefile.inc

SRCDIR=./src
OBJDIR=./obj
LIBDIR=./lib
TSTDIR=./test
BENCHDIR=./bench

TESTLIB=common
TESTFLAGS=

#BENCHLIB=benchmark-gettime
#BENCHFLAGS=-Wl,--no-as-needed -Wl,-lrt
BENCHLIB=benchmark-custom
BENCHFLAGS=-Wl,--no-as-needed

# For file naming
VERSION=$(shell date +%Y%m%d)
DIST=mkhpack-$(VERSION)

# Names of target libraries
NAMES = hpack

TEST_NAMES  = huffman hpack
BENCH_NAMES = huffman

HUFF_CODES = $(SRCDIR)/huffman_codes.inc

# Headers & Sources for building target libraries
hpack_HEADER = $(SRCDIR)/hpack.h
hpack_SOURCE = $(SRCDIR)/hpack.c
hpack_DEPS   = $(HUFF_CODES)

# Lists ...
OBJECTS :=
HEADERS :=
LIBS    :=

TEST_OBJECTS  :=
BENCH_OBJECTS :=
TESTS   :=
BENCHES :=

#
# Default rule
#

.PHONY: all
all: lib

#
# Libraries
#

define LIBRARY_RULES
ifeq (,$$(findstring /$(1).o,$$(OBJECTS)))
	OBJECTS += $(OBJDIR)/$(1).o
	HEADERS += $(LIBDIR)/$(1).h
	LIBS += $(LIBDIR)/$(1).so
endif
# Object
$(OBJDIR)/$(1).o: $$($(1)_SOURCE) $$($(1)_HEADER) $$($(1)_DEPS)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@
# Shared object
$(LIBDIR)/$(1).so: $(OBJDIR)/$(1).o
	$$(CC) -shared $$< -o $$@
endef
$(foreach lib,$(NAMES),$(eval $(call LIBRARY_RULES,$(lib))))

#
# Library header
#

$(LIBDIR)/%.h: $(SRCDIR)/%.h
	$(CP) $< $@

#
# The distribution
#

DISTFILE=$(DIST).tar.gz
$(DISTFILE): lib
	tar cvzf $(DISTFILE) --transform='s,$(LIBDIR)/,$(DIST)/,' $(HEADERS) $(LIBS)

#
# Huffman Codes
#

$(HUFF_CODES): huffman_codes.rb
	ruby $< > $@

#
# Tests
#

TEST_OBJECTS += $(TSTDIR)/$(TESTLIB).o
$(TSTDIR)/$(TESTLIB).o: $(TSTDIR)/$(TESTLIB).c
	$(CC) $(OBJ_CFLAGS) $(CFLAGS) -c $< -o $@

define TEST_RULES
ifeq (,$$(findstring $(1),$$(TESTS)))
	TEST_OBJECTS += $(1).o
	TESTS        += $(1)
endif
# Object
$(1).o: $(1).c $$(HEADERS)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@
# Exe
$(1): $(1).o $$(OBJECTS) $(TSTDIR)/$(TESTLIB).o
	$$(CC) $$^ -o $$@
endef
$(foreach tst,$(TEST_NAMES),$(eval $(call TEST_RULES,$(TSTDIR)/test-$(tst))))

#
# Benchmarks
#

BENCH_OBJECTS += $(BENCHDIR)/$(BENCHLIB).o
$(BENCHDIR)/$(BENCHLIB).o: $(BENCHDIR)/$(BENCHLIB).c
	$(CC) $(OBJ_CFLAGS) $(CFLAGS) $(BENCH_FLAGS) -c $< -o $@

define BENCH_RULES
ifeq (,$$(findstring $(1),$$(BENCHES)))
	BENCH_OBJECTS += $(1).o
	BENCHES       += $(1)
endif
# Object
$(1).o: $(1).c $$(HEADERS)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@
# Exe
$(1): $(1).o $$(OBJECTS) $(BENCHDIR)/$(BENCHLIB).o
	$$(CC) $$^ -o $$@
endef
$(foreach bnch,$(BENCH_NAMES),$(eval $(call BENCH_RULES,$(BENCHDIR)/bench-$(bnch))))

#
# Typical targets
#

.PHONY: lib dist clean distclean

lib: $(HEADERS) $(LIBS)

dist: $(DISTFILE)

clean:
	-rm $(OBJECTS) $(LIBS) $(HEADERS) $(TESTS) $(TEST_OBJECTS) $(BENCHES) $(BENCH_OBJECTS)

distclean:
	-rm $(DISTFILE)


.PHONY: tests check benchmarks bench

tests: $(TESTS)
check: tests
	@$(foreach t,$(TESTS),echo "$(t)"; ./$(t) > /dev/null; echo "$$? test failures";) echo "Done"

benchmarks: $(BENCHES)
bench: benchmarks
	@$(foreach b,$(BENCHES),echo "$(b)"; ./$(b) 2> /dev/null) echo "Done"

.PHONY: always
always:
	true
