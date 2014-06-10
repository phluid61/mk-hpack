.SUFFIXES:
CC=/usr/bin/gcc
CFLAGS=-Wall -O1
CP=/bin/cp

SRCDIR=./src
LIBDIR=./lib
TSTDIR=./test

# For file naming
VERSION=$(shell date +%Y%m%d)
DIST=hpack-$(VERSION)

# Names of target libraries
NAMES := huffman

# Headers & Objects for using target libraries
huffman_HEADERS = $(LIBDIR)/huffman.h
huffman_OBJECTS = $(LIBDIR)/huffman.o

# Headers & Sources for building target libraries
huffman_HEADER = $(SRCDIR)/huffman.h
huffman_SOURCE = $(SRCDIR)/huffman.c

# Lists ...
OBJECTS :=
HEADERS :=
TEST_OBJECTS :=
TESTS :=


#
# Default rule
#

.PHONY: all
all: lib


#
# Libraries and tests
#

define LIBRARY_RULES

ifeq (,$$(findstring $(1).o,$$(OBJECTS)))
	OBJECTS += $(LIBDIR)/$(1).o
	HEADERS += $(LIBDIR)/$(1).h
endif
$(LIBDIR)/$(1).o: $$($(1)_SOURCE) $$($(1)_HEADER)
	$$(CC) $$(CFLAGS) -c $$< -o $$@

TEST_OBJECTS += $(TSTDIR)/test-$(1).o
$(TSTDIR)/test-$(1).o: $(TSTDIR)/test-$(1).c $$($(1)_HEADERS)
	$$(CC) $$(CFLAGS) -c $$< -o $$@

TESTS += $(TSTDIR)/test-$(1)
$(TSTDIR)/test-$(1): $(TSTDIR)/test-$(1).o $$($(1)_OBJECTS)
	$$(CC) $$(CFLAGS) $$^ -o $$@

endef
$(foreach lib,$(NAMES),$(eval $(call LIBRARY_RULES,$(lib))))


$(LIBDIR)/%.h: $(SRCDIR)/%.h
	$(CP) $< $@


#
# The distribution
#

DISTFILE=$(DIST).tar.gz
$(DISTFILE): lib
	tar cvzf $(DISTFILE) --transform='s,$(LIBDIR)/,$(DIST)/,' $(HEADERS) $(OBJECTS)


#
# Non-standard targets
#

.PHONY: lib tests

lib: $(HEADERS) $(OBJECTS)

tests: $(TESTS)


#
# Standard targets
#

.PHONY: dist check clean

dist: $(DISTFILE)

check: tests
	@$(foreach t,$(TESTS),echo "$(t)"; ./$(t) > /dev/null; echo "$$? test failures";) echo "Done"

clean:
	-rm $(OBJECTS) $(HEADERS) $(TESTS) $(TEST_OBJECTS) $(DISTFILE)

