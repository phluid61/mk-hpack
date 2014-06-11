
#include <stddef.h>
#include <stdint.h>

/** The type of integer that can be en/decoded by hpack_en/decode_int() */
#define HPACK_INT_T uint64_t

/**
 * Set by hpack_encode_int() or hpack_encode_str().
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 * - 4: invalid prefix_bits, or invalid prefix
 *
 */
int hpack_encoder_error;

/**
 * Encode an integer as a sequence of bytes.
 *
 * If an error occurs, returns the number of bytes written so far,
 * and sets +hpack_encoder_error+ to one of the following values:
 *
 * - 0: no error
 * - 1: overflow, +buff+ full
 * - 4: invalid +prefix_bits+, or invalid +prefix+
 *
 * @param HPACK_INT_T i       integer to encode
 * @param size_t    prefix_bits  bit-width of prefix (1 to 8 inclusive)
 * @param uint8_t   prefix    first byte, with bits before the prefix preloaded
 * @param uint8_t*  buff      pointer to start of output buffer
 * @param size_t    n         number of bytes allocated to output buffer
 * @param size_t*   produced  used to report number of bytes written, may be NULL
 * @return size_t  number of encoded bytes written to output buffer (same as +produced+)
 */
size_t hpack_encode_int(
		HPACK_INT_T i, size_t prefix_bits, uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
);


int hpack_decoder_error;

/**
 * Decode an integer from a sequence of bytes.
 *
 * If an error occurs, returns an integer with all its set,
 * and sets +hpack_decoder_error+ to one of the following values:
 *
 * - 0: no error
 * - 1: overflow, the encoded integer is too large to represent in +i+
 * - 2: truncated, +buff+ exhausted mid-integer
 * - 4: invalid +prefix_bits+, or invalid +prefix+
 *
 * @param uint8_t*  buff      pointer to start of encoded bytes
 * @param size_t    bytesize  number of encoded bytes to decode
 * @param size_t*   consumed  used to report number of bytes read, may be NULL
 * @param size_t    prefix_bits  bit-width of prefix (1 to 8 inclusive)
 * @param HPACK_INT_T* i      pointer to the decoded integer, may be NULL
 * @param uint8_t*  prefix    first byte with only bits before the prefix set, may be NULL
 * @return HPACK_INT_T  the decoded integer (same as +i+, unless an error occurs)
 */
HPACK_INT_T hpack_decode_int(
		uint8_t *buff, size_t bytesize, size_t *consumed,
		size_t prefix_bits,
		HPACK_INT_T *i, uint8_t *prefix
);

