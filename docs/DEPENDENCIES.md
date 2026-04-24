# Dependencies

## Runtime Dependencies

mkhpack has **no runtime dependencies** beyond the C standard library. The
shared library (`libmkhpack.so`) is self-contained.

## Build Dependencies

| Name | Purpose |
|------|---------|
| GCC | C compiler (path configured in `Makefile.inc`) |
| GNU Make | Build orchestration |
| Ruby | Code generation — runs `huffman_codes.rb` to produce `src/huffman_codes.inc` |
| ar | Creates the static library (`libmkhpack.a`) |

## Packaging Dependencies

### Debian

| Name | Version | Purpose |
|------|---------|---------|
| `debhelper-compat` | = 13 | Debian build helper framework |
| `ruby` | any | Huffman table generation during build |
| `build-essential` | any | GCC, make, and related tools (meta-package) |

### RPM

| Name | Purpose |
|------|---------|
| `gcc` | C compiler |
| `make` | Build orchestration |
| `ruby` | Huffman table generation during build |

## CI/CD Dependencies

The release workflow (`release.yml`) installs these packages on `ubuntu-latest`:

- `build-essential`
- `debhelper`
- `ruby`

The GitHub Pages workflow (`update-pages.yml`) additionally requires:

- Ruby 3.3 (via `ruby/setup-ruby@v1`)
- Bundler and the project's `gh-pages` branch Gemfile

## Test Dependencies

Tests are written using a custom harness (`test/common.{c,h}`) with no external
test framework.

## Benchmark Dependencies

Benchmarks use one of two timing harnesses, both included in the repository:

- `benchmark-custom.c` — uses the x86 RDTSC instruction (default)
- `benchmark-gettime.c` — uses POSIX `clock_gettime`

No external benchmark libraries are required.

## Development Tool Dependencies

| Name | Purpose |
|------|---------|
| `pkg-config` | Optional; consumers use it to discover compiler/linker flags |
| `sed` | Used by Makefile to generate `lib/mkhpack.pc` from template |
| `ln` | Used by Makefile to create shared library symlinks |
