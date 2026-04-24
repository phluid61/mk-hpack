# Developer Guide

## Prerequisites

- **GCC** — the Makefile hardcodes `/usr/bin/gcc` (configurable in `Makefile.inc`)
- **GNU Make**
- **Ruby** — required to regenerate the Huffman code tables (`huffman_codes.rb`)
- **ar** — for building the static library (path configured in `Makefile.inc`)
- **pkg-config** — optional, for consumers linking against the installed library

On Debian/Ubuntu:

```sh
sudo apt-get install build-essential ruby
```

## Getting Started

```sh
git clone https://github.com/phluid61/mk-hpack.git
cd mk-hpack
make          # builds shared and static libraries into lib/
make check    # builds and runs the test suite
```

## Project Structure

```text
src/                Source and headers (the library itself)
  mkhpack.h           Public API header
  mkhpack_errors.h    Error code definitions
  mkhpack.c           Single-file implementation
  huffman_codes.inc   Generated Huffman tables (do not edit by hand)
lib/                Build output (header, shared objects, static library, pkg-config)
obj/                Intermediate object files
test/               Test executables and sources
  common.{c,h}       Shared test helpers (dump, test, test_err)
  test-huffman.c      Huffman encode/decode tests
  test-mkhpack.c      Integer encode/decode and string encode tests
bench/              Benchmark executables and sources
  benchmark.h         Benchmark harness interface
  benchmark-custom.c  RDTSC-based timing implementation (default)
  benchmark-gettime.c clock_gettime-based alternative
  bench-huffman.c     Huffman encode/decode benchmarks
  bench-mkhpack.c     Placeholder for integer benchmarks
debian/             Debian packaging files
huffman_codes.rb    Ruby script that generates src/huffman_codes.inc
str.h               Lightweight byte-string struct used in tests and benchmarks
mkhpack.pc.in       pkg-config template
mkhpack.spec        RPM spec file
```

## Development Workflow

### Branching model

This repository follows git-flow:

- `development` — default branch; feature branches merge here
- `main` — release branch; receives merges from `development`
- `feature/*` — short-lived branches off `development`

### Build targets

| Command | Purpose |
|---------|---------|
| `make` | Build shared and static libraries into `lib/` |
| `make check` | Build the library and run all tests |
| `make bench` | Build the library and run benchmarks |
| `make dist` | Create a source distribution tarball |
| `make bdist` | Create a binary distribution tarball |
| `make install` | Install to `PREFIX` (default `/usr/local`) |
| `make clean` | Remove build artefacts |
| `make distclean` | Remove distribution tarballs |

### Code generation

`huffman_codes.rb` generates `src/huffman_codes.inc` from the Huffman table
embedded as a `__END__` data section in the script. The Makefile rebuilds the
`.inc` file automatically when the Ruby script changes. Do not edit
`huffman_codes.inc` by hand.

## Coding Conventions

- Single-file implementation: all library code lives in `src/mkhpack.c`
- Public API declared in `src/mkhpack.h`; error codes in `src/mkhpack_errors.h`
- Object files are compiled with `-fPIC` for shared library compatibility
- All public functions return an `int` error code (0 = success)
- Optional output parameters accept `NULL` (the function uses a local variable internally)
- The integer type `MKHPACK_INT_T` is defined as `uint64_t`

## Testing

### Framework

Tests use a custom harness defined in `test/common.{c,h}`. There is no external
test framework dependency.

### Running tests

```sh
make check
```

Each test executable returns its failure count as the exit code. `make check`
runs all test executables and prints the failure count for each.

### Test structure

- `test/test-huffman.c` — tests Huffman encoding and decoding with known input/output
  pairs, plus error cases (truncated input, EOS symbol)
- `test/test-mkhpack.c` — tests integer encoding/decoding with various prefix widths,
  raw string encoding, and Huffman-flagged string encoding

### Adding new tests

Follow the pattern in the existing test files:

1. Define input/output pairs as `str` arrays (using `STR_C()` from `str.h`)
2. Call `test()` for expected-success cases or `test_err()` for expected-error cases
3. Sum the return values and return the total from `main()`
4. Add the test name to `TEST_NAMES` in `Makefile`

## Benchmarks

```sh
make bench
```

The default benchmark harness (`benchmark-custom.c`) uses the RDTSC instruction
to measure CPU cycles. An alternative `clock_gettime`-based harness is available
in `benchmark-gettime.c`; switch by editing the `BENCHLIB` variable in `Makefile`.
