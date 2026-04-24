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

# Version
VERSION=2.0.0
SOMAJOR=$(word 1,$(subst ., ,$(VERSION)))
DIST=mkhpack-$(VERSION)

# Install paths
PREFIX?=/usr/local

# Names of target libraries
NAMES = mkhpack

TEST_NAMES  = huffman mkhpack
BENCH_NAMES = huffman

HUFF_CODES = $(SRCDIR)/huffman_codes.inc

# Headers & Sources for building target libraries
mkhpack_LIBNAME = mkhpack
mkhpack_HEADER  = $(SRCDIR)/mkhpack.h
mkhpack_SOURCE  = $(SRCDIR)/mkhpack.c
mkhpack_DEPS    = $(HUFF_CODES) \
                  $(SRCDIR)/mkhpack_str.inc.c \
                  $(SRCDIR)/mkhpack_header.inc.c \
                  $(SRCDIR)/mkhpack_header_table.inc.c \
                  $(SRCDIR)/mkhpack_header_list.inc.c \
                  $(SRCDIR)/mkhpack_static_table.inc.c \
                  $(SRCDIR)/mkhpack_decode_context.inc.c \
                  $(SRCDIR)/mkhpack_encode_context.inc.c

# Lists ...
OBJECTS      :=
HEADERS      :=
LIBS         :=
LIB_SOLINKS  :=
LIB_DEVLINKS :=
STATIC_LIBS  :=

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
	OBJECTS      += $(OBJDIR)/$(1).o
	HEADERS      += $(LIBDIR)/$(1).h
	LIBS         += $(LIBDIR)/lib$$($(1)_LIBNAME).so.$(VERSION)
	LIB_SOLINKS  += $(LIBDIR)/lib$$($(1)_LIBNAME).so.$(SOMAJOR)
	LIB_DEVLINKS += $(LIBDIR)/lib$$($(1)_LIBNAME).so
	STATIC_LIBS  += $(LIBDIR)/lib$$($(1)_LIBNAME).a
endif
# Object
$(OBJDIR)/$(1).o: $$($(1)_SOURCE) $$($(1)_HEADER) $$($(1)_DEPS)
	$$(CC) $$(OBJ_CFLAGS) $$(CFLAGS) -c $$< -o $$@
# Shared object (versioned, with SONAME)
$(LIBDIR)/lib$$($(1)_LIBNAME).so.$(VERSION): $(OBJDIR)/$(1).o
	$$(CC) -shared -Wl,-soname,lib$$($(1)_LIBNAME).so.$(SOMAJOR) $$< -o $$@
# SONAME symlink
$(LIBDIR)/lib$$($(1)_LIBNAME).so.$(SOMAJOR): $(LIBDIR)/lib$$($(1)_LIBNAME).so.$(VERSION)
	ln -sf $$(notdir $$<) $$@
# Development symlink
$(LIBDIR)/lib$$($(1)_LIBNAME).so: $(LIBDIR)/lib$$($(1)_LIBNAME).so.$(SOMAJOR)
	ln -sf $$(notdir $$<) $$@
# Static library
$(LIBDIR)/lib$$($(1)_LIBNAME).a: $(OBJDIR)/$(1).o
	$$(AR) rcs $$@ $$<
endef
$(foreach lib,$(NAMES),$(eval $(call LIBRARY_RULES,$(lib))))

#
# Library header
#

$(LIBDIR)/%.h: $(SRCDIR)/%.h
	$(CP) $< $@

#
# Distributions
#

SDISTFILE=$(DIST)-src.tar.gz
BDISTFILE=$(DIST).tar.gz

SDIST_FILES = src/mkhpack.c src/mkhpack.h src/mkhpack_errors.h \
              src/mkhpack_str.inc.c src/mkhpack_header.inc.c \
              src/mkhpack_header_table.inc.c src/mkhpack_header_list.inc.c \
              src/mkhpack_static_table.inc.c \
              src/mkhpack_decode_context.inc.c src/mkhpack_encode_context.inc.c \
              huffman_codes.rb str.h mkhpack.pc.in \
              Makefile Makefile.inc LICENSE README.md

$(SDISTFILE):
	tar czf $(SDISTFILE) --transform='s,^,$(DIST)/,' $(SDIST_FILES)

$(BDISTFILE): lib
	tar czf $(BDISTFILE) \
		--transform='s,^$(LIBDIR)/,,SH' \
		--transform='s,^,$(DIST)/,SH' \
		$(LIBS) $(LIB_SOLINKS) $(LIB_DEVLINKS) $(STATIC_LIBS) $(HEADERS) $(PCFILE) LICENSE README.md

#
# Huffman Codes
#

$(HUFF_CODES): huffman_codes.rb
	ruby $< > $@

#
# pkg-config
#

PCFILE=$(LIBDIR)/mkhpack.pc

$(PCFILE): mkhpack.pc.in Makefile
	sed -e 's,@PREFIX@,$(PREFIX),g' -e 's,@VERSION@,$(VERSION),g' $< > $@

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

.PHONY: lib dist bdist clean distclean install uninstall

lib: $(HEADERS) $(LIBS) $(LIB_SOLINKS) $(LIB_DEVLINKS) $(STATIC_LIBS) $(PCFILE)

dist: $(SDISTFILE)
bdist: $(BDISTFILE)

clean:
	-rm -f $(OBJECTS) $(STATIC_LIBS) $(HEADERS) $(PCFILE) $(TESTS) $(TEST_OBJECTS) $(BENCHES) $(BENCH_OBJECTS)
	-rm -f $(LIBDIR)/*.so $(LIBDIR)/*.so.*

distclean:
	-rm -f $(SDISTFILE) $(BDISTFILE)

install: lib
	install -d $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(PREFIX)/lib/pkgconfig
	install -d $(DESTDIR)$(PREFIX)/include
	install -m 755 $(LIBS) $(DESTDIR)$(PREFIX)/lib/
	cp -d $(LIB_SOLINKS) $(LIB_DEVLINKS) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(STATIC_LIBS) $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(HEADERS) $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(PCFILE) $(DESTDIR)$(PREFIX)/lib/pkgconfig/

uninstall:
	-rm $(addprefix $(DESTDIR)$(PREFIX)/lib/,$(notdir $(LIBS) $(LIB_SOLINKS) $(LIB_DEVLINKS) $(STATIC_LIBS)))
	-rm $(addprefix $(DESTDIR)$(PREFIX)/include/,$(notdir $(HEADERS)))
	-rm $(DESTDIR)$(PREFIX)/lib/pkgconfig/mkhpack.pc


.PHONY: check bench

check: $(TESTS)
	@$(foreach t,$(TESTS),echo "$(t)"; ./$(t) > /dev/null; echo "$$? test failures";) echo "Done"

bench: $(BENCHES)
	@$(foreach b,$(BENCHES),echo "$(b)"; ./$(b) 2> /dev/null) echo "Done"
