# Business Rules

The encoding and decoding rules in mkhpack are derived directly from
[RFC 7541](https://www.rfc-editor.org/rfc/rfc7541.html). They are not
application-specific business rules but rather protocol-level constraints
mandated by the specification.

## Integer Encoding (RFC 7541 §5.1)

### Rule: Prefix width must be between 1 and 8 bits

**Description:** The `prefix_bits` parameter must be in the range 1–8 inclusive.
Values outside this range are rejected with `ERROR_BAD_PREFIX`. This corresponds
to the valid prefix sizes defined in RFC 7541 §5.1.

**Implementation:** `mkhpack_encode_int()` and `mkhpack_decode_int()` in `src/mkhpack.c`

**Related Constants:** `ERROR_BAD_PREFIX = 4` in `src/mkhpack_errors.h`

### Rule: Prefix high bits must not overlap the encoded value

**Description:** When encoding, the `prefix` byte must have all bits in the
low `prefix_bits` positions set to zero. If any of those bits are set, the
function returns `ERROR_BAD_PREFIX`. This ensures the caller has correctly
partitioned the byte between the HPACK integer and any preceding flags.

**Implementation:** `mkhpack_encode_int()` in `src/mkhpack.c`, via the
`PREFIX_MASK()` macro check

### Rule: Integer overflow detection on decode

**Description:** During decoding, if a continuation byte would cause the
accumulated integer to exceed `UINT64_MAX`, the function returns
`ERROR_OVERFLOW` rather than silently wrapping. This protects callers from
processing corrupted or malicious input.

**Implementation:** `mkhpack_decode_int()` in `src/mkhpack.c`

**Related Constants:** `MAX_MKHPACK_INT` (defined as `UINT64_C(-1)`) in `src/mkhpack.c`

## Huffman Coding (RFC 7541 §5.2, Appendix B)

### Rule: EOS symbol is invalid in encoded data

**Description:** The Huffman table includes an EOS (End of String) symbol at
index 256. If this symbol is encountered during decoding, the function returns
`ERROR_EOS`. Per RFC 7541 §5.2, a Huffman-encoded string literal containing
the EOS symbol must be treated as a decoding error.

**Implementation:** `huffman_decode()` in `src/mkhpack.c`

**Related Constants:** `ERROR_EOS = 3` in `src/mkhpack_errors.h`

### Rule: Padding must consist of EOS prefix bits

**Description:** When a Huffman-encoded string does not end on a byte boundary,
the remaining bits are padded with the most significant bits of the EOS symbol
(i.e. all `1` bits). The decoder validates this by checking that the trailing
bits of the final byte match the expected padding mask. Invalid padding results
in `ERROR_TRUNCATED`.

**Implementation:** `huffman_decode()` in `src/mkhpack.c` (the `mask` variable
tracks the expected padding pattern; the `(byte & mask) == mask` check validates it)

### Rule: Encoder pads with EOS bits

**Description:** When the Huffman encoder finishes and there are fewer than 8
bits remaining in the output queue, it pads the final byte by shifting the
queued bits left and filling the remaining positions with `1` bits (the EOS
prefix). This is the encoding counterpart to the padding validation rule above.

**Implementation:** `huffman_encode()` in `src/mkhpack.c`

## String Encoding (RFC 7541 §5.2)

### Rule: Automatic Huffman optimisation

**Description:** `mkhpack_encode_str()` compares the Huffman-encoded length
(via `huffman_length()`) against the raw string length. If Huffman encoding
produces a shorter result, it is used; otherwise the string is sent raw. This
implements the recommendation in RFC 7541 §5.2 that an encoder should use
whichever representation is more compact.

**Implementation:** `mkhpack_encode_str()` in `src/mkhpack.c`

### Rule: Huffman flag in string length prefix

**Description:** The high bit (0x80) of the string length's first byte indicates
whether the string payload is Huffman-encoded. `mkhpack_encode_raw_str()` clears
this bit; `mkhpack_encode_huff_str()` sets it. `mkhpack_decode_str()` reads this
bit to determine how to decode the payload.

**Implementation:** `mkhpack_encode_str_()` (static helper), `mkhpack_decode_str()` in `src/mkhpack.c`

**Related Constants:** `HUFFMAN_FLAG = 0x80`, `HUFFMAN_PREFIX = 7` in `src/mkhpack.c`
