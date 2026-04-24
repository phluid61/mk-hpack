# Configuration

mkhpack has no runtime configuration. All configuration is at build time, via
Makefile variables.

## Build-Time Configuration

### Compiler and tool paths (`Makefile.inc`)

| Variable | Default | Purpose |
|----------|---------|---------|
| `CC` | `/usr/bin/gcc` | C compiler |
| `AR` | `/usr/bin/ar` | Archive tool for static libraries |
| `CP` | `/bin/cp` | File copy command |
| `CFLAGS` | `-Wall` | Compiler warning flags |
| `OBJ_CFLAGS` | `-fPIC -Wl,--relocatable` | Position-independent code flags for shared library objects |

To override, edit `Makefile.inc` or pass values on the command line:

```sh
make CC=/usr/bin/clang CFLAGS="-Wall -O2"
```

### Version and install paths (`Makefile`)

| Variable | Default | Purpose |
|----------|---------|---------|
| `VERSION` | `2.0.0` | Library version; determines SONAME and tarball names |
| `PREFIX` | `/usr/local` | Installation prefix (overridable via environment or command line) |
| `DESTDIR` | *(empty)* | Staging directory for packaging (prepended to all install paths) |

### Benchmark harness (`Makefile`)

| Variable | Default | Alternative |
|----------|---------|-------------|
| `BENCHLIB` | `benchmark-custom` | `benchmark-gettime` |
| `BENCHFLAGS` | `-Wl,--no-as-needed` | `-Wl,--no-as-needed -Wl,-lrt` |

The default harness (`benchmark-custom`) uses the x86 RDTSC instruction for
cycle-accurate timing. The alternative (`benchmark-gettime`) uses POSIX
`clock_gettime` and may be more portable.

## pkg-config

After installation, consumers can discover compiler and linker flags via
pkg-config. The template is `mkhpack.pc.in`; the build substitutes `@PREFIX@`
and `@VERSION@` to produce `lib/mkhpack.pc`.

```sh
pkg-config --cflags mkhpack    # → -I/usr/local/include
pkg-config --libs mkhpack      # → -L/usr/local/lib -lmkhpack
```

## Installed File Layout

After `make install PREFIX=/usr/local`:

```text
/usr/local/
  include/
    mkhpack.h
  lib/
    libmkhpack.so.2.0.0      Versioned shared library
    libmkhpack.so.2           SONAME symlink
    libmkhpack.so             Development symlink
    libmkhpack.a              Static library
    pkgconfig/
      mkhpack.pc              pkg-config metadata
```
