.SUFFIXES:
CC=/usr/bin/gcc
CFLAGS=-Wall -O1
CP=/bin/cp

SRCDIR=./src
LIBDIR=./lib
TSTDIR=./test

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
TEST_OBJECTS :=
TESTS :=


#
# Default rule
#

.PHONY: all
all: objs tests


#
# Libraries and tests
#

define LIBRARY_RULES

ifeq (,$$(findstring $(1).o,$$(OBJECTS)))
	OBJECTS += $(LIBDIR)/$(1).o
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

#
# Headers for libraries
#

$(LIBDIR)/%.h: $(SRCDIR)/%.h
	$(CP) $< $@


#
# Working with lists
#

.PHONY: objs
objs: $(OBJECTS)

.PHONY: tests
tests: $(TESTS)

.PHONY: run-tests
run-tests: tests
	@$(foreach t,$(TESTS),echo "$(t)"; ./$(t); echo "$$? test failures";) echo "Done"

.PHONY: clean
clean:
	-rm $(TESTS) $(TEST_OBJECTS) $(OBJECTS)

