
#include <stddef.h>
#include <stdint.h>

/** The type of integer that can be en/decoded by hpack_en/decode_int() */
#define HPACK_INT_T uint64_t


/**
 * Encode an integer as a sequence of bytes.
 *
 * If an error occurs, returns one of the following values:
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
 * @param size_t*   produced  used to report number of bytes written
 * @return 
 */
int hpack_encode_int(
		HPACK_INT_T i, size_t prefix_bits, uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
);


/**
 * Decode an integer from a sequence of bytes.
 *
 * If an error occurs, returns one of the following values:
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
 * @param HPACK_INT_T* i      pointer to the decoded integer
 * @param uint8_t*  prefix    first byte with only bits before the prefix set, may be NULL
 * @return 
 */
int hpack_decode_int(
		uint8_t *buff, size_t bytesize, size_t *consumed,
		size_t prefix_bits,
		HPACK_INT_T *i, uint8_t *prefix
);

