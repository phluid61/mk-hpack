#include <stddef.h>
#include <stdint.h>

/**
 * Huffman-encode a sequence of bytes.
 *
 * If an error occurs, returns one of the following values:
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int huffman_encode(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

/**
 * Calculate the length of a Huffman-encode a sequence of bytes.
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @return size_t number of encoded bytes
 */
size_t huffman_length(
		uint8_t *str, size_t bytesize
);

size_t huffman_length(
		uint8_t *str, size_t bytesize
) {
	size_t result = 7;
	while (bytesize > 0) {
		result += HuffmanCodes[*str]; str++; bytesize--;
	}
	return (result / 8);
}

/**
 * Decode a sequence of Huffman-encoded bytes.
 *
 * If an error occurs, returns one of the following values:
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 * - 2: truncated, input buffer exhausted mid-code
 * - 3: an invalid huffman code was encountered (EOS=256)
 *
 * @param uint8_t* huff     pointer to start of Huffman-encoded bytes
 * @param size_t   bytesize number of Huffman-encoded bytes to decode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
size_t huffman_decode(
		uint8_t *huff, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

