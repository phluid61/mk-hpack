HPACK — HTTP/2 Compression
==========================

A C implementation of HPACK: Header Compression for HTTP/2 as specified
in [RFC 7541](https://www.rfc-editor.org/rfc/rfc7541.html).

[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-3.0-4baaaa.svg)](code_of_conduct.md)

## Key Features

- HPACK integer encoding and decoding (RFC 7541 §5.1)
- Huffman string encoding, decoding, and length calculation (RFC 7541 §5.2)
- HPACK string literal encoding and decoding with automatic Huffman optimisation
- Shared library (`.so`), static library (`.a`), and pkg-config support
- No runtime dependencies beyond the C standard library

## Technology Stack

| Component | Detail |
|-----------|--------|
| Language | C (compiled with GCC) |
| Build system | GNU Make |
| Code generation | Ruby (Huffman tables) |
| Packaging | Debian (`.deb`), RPM (`.spec`), tarballs |
| CI/CD | GitHub Actions |
| Licence | ISC |

## Quick Start

```sh
make            # build shared and static libraries into lib/
make check      # build and run tests
make install    # install to /usr/local (or set PREFIX)
```

Link against the library using pkg-config:

```sh
gcc -o myapp myapp.c $(pkg-config --cflags --libs mkhpack)
```

## Usage Examples

These examples use static linking:

### HPACK integer encoding

```c
#include <stdio.h>
#include <stdint.h>
#include "lib/mkhpack.h"

MKHPACK_INT_T i = 987; /* number to encode */
size_t  nbits = 6;     /* number of bits to occupy in the first byte */
uint8_t pbits = 0x40;  /* initial value of the first byte */

#define BUFFER_SIZE 4321
uint8_t buffer[BUFFER_SIZE];
size_t  written;       /* will hold the number of bytes written */

int error = mkhpack_encode_int(i, nbits, pbits, buffer, BUFFER_SIZE, &written);
if (error) {
    fprintf(stderr, "Error encoding integer: %d\n", error);
} else {
    printf("Encoded integer in %zu bytes\n", written);
}
```

```c
#include <stdio.h>
#include <stdint.h>
#include "lib/mkhpack.h"

#define CODE_LENGTH 10
uint8_t code[CODE_LENGTH] = "\xFF\xF2\x81\xC0\x80\x01XYZ";
size_t  consumed;      /* will hold the number of bytes read */
size_t  nbits = 6;     /* number of bits to occupy in the first byte */

MKHPACK_INT_T i;       /* decoded integer */
uint8_t pbits;         /* will hold remainder of the first byte */

int error = mkhpack_decode_int(code, CODE_LENGTH, &consumed, nbits, &i, &pbits);
if (error) {
    fprintf(stderr, "Error decoding integer: %d\n", error);
} else {
    printf("Decoded integer[%lu] from %zu bytes, prefix was %02x\n",
        (unsigned long)i, consumed, pbits);
}
```

### Huffman string encoding

```c
#include <stdio.h>
#include <stdint.h>
#include "lib/mkhpack.h"

#define STRING_LENGTH 16
uint8_t string[STRING_LENGTH] = "www.example.com";

#define BUFFER_SIZE 4321
uint8_t buffer[BUFFER_SIZE];
size_t  written;      /* will hold the number of bytes written */

int error = huffman_encode(string, STRING_LENGTH, NULL, buffer, BUFFER_SIZE, &written);
if (error) {
    fprintf(stderr, "Error encoding string: %d\n", error);
} else {
    printf("Encoded %d-byte string in %zu bytes\n", STRING_LENGTH, written);
}
```

```c
#include <stdio.h>
#include <stdint.h>
#include "lib/mkhpack.h"

#define CODE_LENGTH 16
uint8_t code[CODE_LENGTH] = "\xE7\xCF\x9B\xEB\xE8\x9B\x6F\xB1\x6F\xA9\xB6\xFFXYZ";
size_t  consumed;     /* will hold the number of bytes read */

#define BUFFER_SIZE 4321
uint8_t buffer[BUFFER_SIZE];
size_t  written;      /* will hold the number of bytes written */

int error = huffman_decode(code, CODE_LENGTH, &consumed, buffer, BUFFER_SIZE, &written);
if (error) {
    fprintf(stderr, "Error decoding string: %d\n", error);
} else {
    printf("Decoded %zu-byte string from %zu bytes\n", written, consumed);
}
```

## Project Links

| Resource | URL |
|----------|-----|
| Repository | [github.com/phluid61/mk-hpack](https://github.com/phluid61/mk-hpack) |
| Issue Tracker | [GitHub Issues](https://github.com/phluid61/mk-hpack/issues) |
| GitHub Pages | [phluid61.github.io/mk-hpack](https://phluid61.github.io/mk-hpack/) |
| Documentation | [docs/](docs/) |

## Documentation Guide

| Audience | Start Here |
|----------|------------|
| New developers | [Developer Guide](docs/DEVELOPER_GUIDE.md) |
| Architecture overview | [Architecture](docs/ARCHITECTURE.md) |
| API reference | [API](docs/API.md) |
| Build & release | [Deployment](docs/DEPLOYMENT.md) |
| Configuration | [Configuration](docs/CONFIGURATION.md) |
| All documents | [Documentation Index](docs/README.md) |

## Contributor Code of Conduct

This repository is subject to a [Contributor Code of Conduct](code_of_conduct.md)
adapted from the [Contributor Covenant][cc], version 3.0, available at
<https://www.contributor-covenant.org/version/3/0/>


[cc]: https://www.contributor-covenant.org


## Licence

This project is licensed under the ISC licence. See [LICENSE](LICENSE)
for details.

