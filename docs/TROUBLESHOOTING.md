# Troubleshooting

## Build Issues

### `ruby: command not found`

The Huffman table generation step requires Ruby. Install it:

```sh
sudo apt-get install ruby       # Debian/Ubuntu
sudo dnf install ruby           # Fedora/RHEL
```

### `gcc: command not found` or wrong compiler

The Makefile expects GCC at `/usr/bin/gcc`. If your compiler is elsewhere,
edit `Makefile.inc` or override on the command line:

```sh
make CC=/path/to/gcc
```

### Linker errors with `-fPIC`

Object files must be compiled with `-fPIC` for the shared library. This is the
default in `Makefile.inc` (`OBJ_CFLAGS=-fPIC -Wl,--relocatable`). If you have
overridden `OBJ_CFLAGS`, ensure `-fPIC` is still included.

### `huffman_codes.inc` out of date or missing

The Makefile regenerates `src/huffman_codes.inc` automatically when
`huffman_codes.rb` changes. If the file is missing or corrupt, regenerate it
manually:

```sh
ruby huffman_codes.rb > src/huffman_codes.inc
```

Then rebuild:

```sh
make clean && make
```

## Test Failures

### Reading test output

Each test executable prints `PASS` (green) or an error message (red) for each
test case, followed by a summary line. The exit code is the number of failures.

`make check` runs all test executables and prints the failure count:

```text
test/test-huffman
0 test failures
test/test-mkhpack
0 test failures
Done
```

A non-zero failure count indicates which test executable had problems. Run it
directly for detailed output:

```sh
make lib
make test/test-huffman
./test/test-huffman
```

### Common test failure patterns

- **`error: 1`** — `ERROR_OVERFLOW`: the internal test buffer (1024 bytes) was
  too small for the test case. This would indicate a logic change that increased
  output size unexpectedly.
- **`error: 2`** — `ERROR_TRUNCATED`: the test input was consumed before the
  value was fully decoded. Check whether `huffman_codes.inc` is up to date.
- **`mismatch at byte N`** — the encoded/decoded output differs from the
  expected value at byte position N. The test prints the input (`<`), actual
  output (`>`), and expected output (`~`) in hex for comparison.

## Benchmark Issues

### RDTSC not available

The default benchmark harness (`benchmark-custom.c`) uses the x86 RDTSC
instruction. On non-x86 platforms or virtualised environments where RDTSC is
unavailable, switch to the `clock_gettime` harness by editing the Makefile:

```makefile
BENCHLIB=benchmark-gettime
BENCHFLAGS=-Wl,--no-as-needed -Wl,-lrt
```

### Benchmark results vary wildly

CPU frequency scaling and other processes can affect results. For more stable
measurements:

1. Disable CPU frequency scaling if possible
2. Run benchmarks on an otherwise-idle system
3. The custom harness attempts to bind to a single CPU core via `sched_setaffinity`

## Runtime Issues

### `ERROR_OVERFLOW` (1) from encoding functions

The output buffer is too small for the encoded data. For integer encoding, the
worst case is 10 bytes (a full 64-bit value with a 1-bit prefix). For Huffman
encoding, the worst case is approximately 4× the input length (for byte values
with 30-bit Huffman codes). For string encoding, add the length prefix overhead.

### `ERROR_TRUNCATED` (2) from decoding functions

The input buffer ended before the encoded value was complete. Ensure the full
encoded sequence is present in the buffer and that `bytesize` accurately
reflects the available data.

### `ERROR_EOS` (3) from `huffman_decode()`

The Huffman-encoded data contained the EOS symbol (code point 256), which is
forbidden in HPACK string data per RFC 7541 §5.2. This typically indicates
corrupted input.

### `ERROR_BAD_PREFIX` (4) from integer functions

Either `prefix_bits` is outside the range 1–8, or the `prefix` byte has bits
set in the region reserved for the integer value. Check that the caller is
correctly partitioning the first byte.

## Packaging Issues

### Debian build fails with missing `debhelper`

```sh
sudo apt-get install debhelper
```

The `debian/control` file requires `debhelper-compat (= 13)`.

### RPM build: spec file version mismatch

The `mkhpack.spec` file has its own `Version:` field. If it does not match the
`VERSION` in `Makefile`, the resulting RPM will have a different version string
from the shared library SONAME. Keep both in sync.

## Log Analysis

> ⚠️ **LOW CONFIDENCE**: mkhpack is a library, not a service, and does not
> produce log files. If a consuming application logs mkhpack errors, consult
> that application's documentation for log file locations.

## Getting Help

- File a bug report: [GitHub Issues](https://github.com/phluid61/mk-hpack/issues/new?template=bug_report.md)
- Request a feature: [GitHub Issues](https://github.com/phluid61/mk-hpack/issues/new?template=feature_request.md)
- RFC 7541 specification: [rfc-editor.org/rfc/rfc7541](https://www.rfc-editor.org/rfc/rfc7541.html)
