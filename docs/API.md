# API

mkhpack is a C library consumed by including `mkhpack.h` and linking against
`libmkhpack`. It does not expose HTTP endpoints, CLI commands, or RPC interfaces.

All functions return an `int` error code. A return value of `0` (`ERROR_NONE`)
indicates success. Error codes are defined in `src/mkhpack_errors.h`.

## Types

### `MKHPACK_INT_T`

Defined as `uint64_t`. This is the integer type used for all HPACK integer
encoding and decoding operations.

## Error Codes

| Code | Constant | Meaning |
|------|----------|---------|
| 0 | `ERROR_NONE` | No error |
| 1 | `ERROR_OVERFLOW` | Output buffer full (encoding) or integer too large to represent (decoding) |
| 2 | `ERROR_TRUNCATED` | Input buffer exhausted mid-value |
| 3 | `ERROR_EOS` | Invalid Huffman code — the EOS symbol (256) was encountered |
| 4 | `ERROR_BAD_PREFIX` | `prefix_bits` out of range (must be 1–8), or `prefix` has bits set in the masked region |

## Integer Functions

### `mkhpack_encode_int()`

Encode an integer as a variable-length byte sequence per RFC 7541 §5.1.

**Signature:** `int mkhpack_encode_int(MKHPACK_INT_T i, size_t prefix_bits, uint8_t prefix, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `i` | `MKHPACK_INT_T` | Integer to encode |
| `prefix_bits` | `size_t` | Bit-width of the prefix field (1 to 8 inclusive) |
| `prefix` | `uint8_t` | First byte with high bits pre-loaded; low `prefix_bits` bits must be zero |
| `buff` | `uint8_t *` | Output buffer (may be `NULL` to calculate length only) |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE`, `ERROR_OVERFLOW`, or `ERROR_BAD_PREFIX`

---

### `mkhpack_decode_int()`

Decode a variable-length integer from a byte sequence per RFC 7541 §5.1.

**Signature:** `int mkhpack_decode_int(uint8_t *buff, size_t bytesize, size_t *consumed, size_t prefix_bits, MKHPACK_INT_T *i, uint8_t *prefix)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `buff` | `uint8_t *` | Pointer to encoded bytes |
| `bytesize` | `size_t` | Number of encoded bytes available |
| `consumed` | `size_t *` | Receives the number of bytes read (may be `NULL`) |
| `prefix_bits` | `size_t` | Bit-width of the prefix field (1 to 8 inclusive) |
| `i` | `MKHPACK_INT_T *` | Receives the decoded integer (may be `NULL`) |
| `prefix` | `uint8_t *` | Receives the high bits of the first byte (may be `NULL`) |

**Returns:** `ERROR_NONE`, `ERROR_OVERFLOW`, `ERROR_TRUNCATED`, or `ERROR_BAD_PREFIX`

## Huffman Functions

### `huffman_encode()`

Huffman-encode a byte sequence using the HPACK Huffman table (RFC 7541 Appendix B).

**Signature:** `int huffman_encode(uint8_t *str, size_t bytesize, size_t *consumed, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `uint8_t *` | Input bytes to encode |
| `bytesize` | `size_t` | Number of input bytes |
| `consumed` | `size_t *` | Receives the number of input bytes read (may be `NULL`) |
| `buff` | `uint8_t *` | Output buffer (may be `NULL` to calculate length only) |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE` or `ERROR_OVERFLOW`

---

### `huffman_decode()`

Decode a Huffman-encoded byte sequence.

**Signature:** `int huffman_decode(uint8_t *huff, size_t bytesize, size_t *consumed, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `huff` | `uint8_t *` | Huffman-encoded input bytes |
| `bytesize` | `size_t` | Number of input bytes |
| `consumed` | `size_t *` | Receives the number of input bytes read (may be `NULL`) |
| `buff` | `uint8_t *` | Output buffer (may be `NULL` to calculate length only) |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE`, `ERROR_OVERFLOW`, `ERROR_TRUNCATED`, or `ERROR_EOS`

---

### `huffman_length()`

Calculate the encoded length of a byte sequence without performing the encoding.

**Signature:** `size_t huffman_length(uint8_t *str, size_t bytesize)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `uint8_t *` | Input bytes |
| `bytesize` | `size_t` | Number of input bytes |

**Returns:** The number of bytes the Huffman encoding would produce.

## String Functions

### `mkhpack_encode_raw_str()`

Encode a raw (non-Huffman) string literal per RFC 7541 §5.2. Prepends the
string length as a 7-bit-prefixed integer with the Huffman flag cleared.

**Signature:** `int mkhpack_encode_raw_str(uint8_t *str, size_t bytesize, size_t *consumed, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `uint8_t *` | Input bytes |
| `bytesize` | `size_t` | Number of input bytes |
| `consumed` | `size_t *` | Receives the number of input bytes read (may be `NULL`) |
| `buff` | `uint8_t *` | Output buffer |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE` or `ERROR_OVERFLOW`

---

### `mkhpack_encode_huff_str()`

Encode a pre-Huffman-encoded string literal per RFC 7541 §5.2. Prepends the
string length as a 7-bit-prefixed integer with the Huffman flag set. The caller
is responsible for Huffman-encoding the payload beforehand.

**Signature:** `int mkhpack_encode_huff_str(uint8_t *str, size_t bytesize, size_t *consumed, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `uint8_t *` | Pre-encoded Huffman bytes |
| `bytesize` | `size_t` | Number of input bytes |
| `consumed` | `size_t *` | Receives the number of input bytes read (may be `NULL`) |
| `buff` | `uint8_t *` | Output buffer |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE` or `ERROR_OVERFLOW`

---

### `mkhpack_encode_str()`

Encode a string literal, automatically choosing Huffman encoding if it results
in a shorter output.

**Signature:** `int mkhpack_encode_str(uint8_t *str, size_t bytesize, size_t *consumed, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `uint8_t *` | Input bytes |
| `bytesize` | `size_t` | Number of input bytes |
| `consumed` | `size_t *` | Receives the number of input bytes read (may be `NULL`) |
| `buff` | `uint8_t *` | Output buffer |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE` or `ERROR_OVERFLOW`

---

### `mkhpack_decode_str()`

Decode an HPACK string literal. Automatically detects whether the string is
Huffman-encoded (via the high bit of the first byte) and decodes accordingly.

**Signature:** `int mkhpack_decode_str(uint8_t *str, size_t bytesize, size_t *consumed, uint8_t *buff, size_t n, size_t *produced)` in `src/mkhpack.c`

**Parameters:**

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `uint8_t *` | Encoded string bytes (length prefix + payload) |
| `bytesize` | `size_t` | Number of input bytes |
| `consumed` | `size_t *` | Receives the number of input bytes read (may be `NULL`) |
| `buff` | `uint8_t *` | Output buffer |
| `n` | `size_t` | Size of output buffer in bytes |
| `produced` | `size_t *` | Receives the number of bytes written (may be `NULL`) |

**Returns:** `ERROR_NONE`, `ERROR_OVERFLOW`, `ERROR_TRUNCATED`, or `ERROR_EOS`
