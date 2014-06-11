#include <stddef.h>
#include <stdint.h>

/**
 * Set by huffman_encode().
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 *
 */
extern int huffman_encoder_error;

/**
 * Huffman-encode a sequence of bytes.
 *
 * If an error occurs, returns the number of bytes written so far,
 * and sets +huffman_encoder_error+ to one of the following values:
 *
 * - 0: no error
 * - 1: overflow, +buff+ full
 * - 2: truncated, +huff+ exhausted mid-code
 * - 3: an invalid huffman code was encountered (EOS=256)
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written, may be NULL
 * @return size_t number of encoded bytes written to output buffer (same as +produced+)
 */
size_t huffman_encode(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

/**
 * Set by huffman_decode().
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 * - 2: truncated, input buffer exhausted mid-code
 * - 3: an invalid huffman code was encountered (EOS=256)
 *
 */
extern int huffman_decoder_error;

/**
 * Decode a sequence of Huffman-encoded bytes.
 *
 * If an error occurs, returns the number of bytes written so far,
 * and sets +huffman_decoder_error+ to one of the following values:
 *
 * - 0: no error
 * - 1: overflow, +buff+ full
 * - 2: truncated, +huff+ exhausted mid-code
 * - 3: an invalid huffman code was encountered (EOS=256)
 *
 * @param uint8_t* huff     pointer to start of Huffman-encoded bytes
 * @param size_t   bytesize number of Huffman-encoded bytes to decode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written, may be NULL
 * @return size_t number of decoded bytes written to output buffer (same as +produced+)
 */
size_t huffman_decode(
		uint8_t *huff, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

