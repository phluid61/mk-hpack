# Agents

This is a C library implementing HPACK (Header Compression for HTTP/2,
[RFC 7541](https://www.rfc-editor.org/rfc/rfc7541.html)).

## Orientation

- Public API: `src/hpack.h` — all exported function signatures and doc comments
- Error codes: `src/hpack_errors.h`
- Implementation: `src/hpack.c` — single-file; includes generated Huffman tables
- Helper type: `str.h` (root) — lightweight byte-string struct used in tests

## Build

The build is GNU Make. Compiler flags and tool paths are in `Makefile.inc`;
everything else is in `Makefile`.

```
make            # build lib/ (shared object + header)
make dist       # build a versioned tarball
make check      # build and run tests
make bench      # build and run benchmarks
make clean      # remove build artefacts
```

There is no CI pipeline for builds or tests. The only GitHub Actions workflow
(`.github/workflows/update-pages.yml`) syncs documentation to `gh-pages`.

## Code generation

`huffman_codes.rb` (Ruby) generates `src/huffman_codes.inc` from the Huffman
table embedded as a `__END__` data section in the same script. The Makefile
rebuilds the `.inc` file automatically when the script changes.

## Tests and benchmarks

- Test sources: `test/test-*.c`, with shared helpers in `test/common.{c,h}`
- Benchmark sources: `bench/bench-*.c`, with harness in `bench/benchmark*.{c,h}`
- Tests return a count of failures to stdout; `make check` runs them all

## Layout

```
src/            Source and headers (the library itself)
lib/            Build output (copied headers + shared objects)
obj/            Intermediate object files
test/           Test executables and sources
bench/          Benchmark executables and sources
```

## Licence

ISC — see `LICENSE`.
