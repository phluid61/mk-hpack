HPACK - HTTP/2 Compression
==========================

This is a basic implementation of HPACK: Header Compression for HTTP/2 as specified
in [RFC 7541](https://www.rfc-editor.org/rfc/rfc7541.html)

[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](code_of_conduct.md)

## To build

    make dist

## To use

These examples use static linking:

### HPACK integer encoding

```c
#include <stdint.h>
#include "lib/hpack.h"

unsigned int i = 987; /* number to encode */
size_t  nbits = 6;    /* number of bits to occupy in the first byte */
uint8_t pbits = 0x40; /* initial value of the first byte */

#define BUFFER_SIZE 4321
uint8_t buffer[BUFFER_SIZE];
size_t  written;      /* will hold the number of bytes written */

written = hpack_encode_int(i, nbits, pbits, buffer, BUFFER_SIZE, NULL);
if (hpack_encoder_error) {
    fprintf(2, "Error encoding integer[%u]: %d\n", i, hpack_encoder_error);
} else {
    printf("Encoded integer[%u] in %d bytes\n", i, written);
}
```

```c
#include <stdint.h>
#include "lib/hpack.h"

#define CODE_LENGTH 10
uint8_t code[CODE_LENGH] = "\xFF\xF2\x81\xC0\x80\x01XYZ";
size_t  read;         /* will hold the number of bytes read */
size_t  nbits = 6;    /* number of bits to occupy in the first byte */

unsigned int i;       /* number to encode */
uint8_t pbits;        /* will hold remainder of the first byte */

i = hpack_decode_int(code, CODE_LENGTH, &read, nbits, NULL, &pbits);
if (hpack_decoder_error) {
    fprintf(2, "Error decoding integer: %d\n", hpack_encoder_error);
} else {
    printf("Decoded integer[%u] from %d bytes, prefix was %02x\n", i, read, pbits);
}
```

### Huffman string encoding

```c
#include <stdint.h>
#include "lib/huffman.h"

#define STRING_LENGTH 16
uint8_t string[STRING_LENGH] = "www.example.com";

#define BUFFER_SIZE 4321
uint8_t buffer[BUFFER_SIZE];
size_t  written;      /* will hold the number of bytes written */

written = huffman_encode(string, STRING_LENGTH, NULL, buffer, BUFFER_SIZE, NULL);
if (huffman_encoder_error) {
    fprintf(2, "Error encoding string: %d\n", huffman_encoder_error);
} else {
    printf("Encoded %d-byte string in %d bytes\n", STRING_LENGTH, written);
}
```

```c
#include <stdint.h>
#include "lib/huffman.h"

#define CODE_LENGTH 16
uint8_t code[CODE_LENGH] = "\xE7\xCF\x9B\xEB\xE8\x9B\x6F\xB1\x6F\xA9\xB6\xFFXYZ";
size_t  read;         /* will hold the number of bytes read */

#define BUFFER_SIZE 4321
uint8_t buffer[BUFFER_SIZE];
size_t  written;      /* will hold the number of bytes written */

written = huffman_decode(code, CODE_LENGTH, &read, buffer, BUFFER_SIZE, NULL);
if (huffman_decoder_error) {
    fprintf(2, "Error decoding string: %d\n", huffman_decoder_error);
} else {
    printf("Decoded %d-byte string from %d bytes\n", written, read);
}
```

## Contributor Code of Conduct

This repository is subject to a [Contributor Code of Conduct](code_of_conduct.md)
adapted from the [Contributor Covenant][cc], version 2.1, available at
<http://contributor-covenant.org/version/2/1>


[cc]: http://contributor-covenant.org


## License

Copyright (c) 2014, 2021, Matthew Kerwin <matthew@kerwin.net.au>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

