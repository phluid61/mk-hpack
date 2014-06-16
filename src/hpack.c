
#include <stddef.h>
#include <stdint.h>
#include "hpack_errors.h"

#define HPACK_INT_T uint64_t
#define MAX_HPACK_INT UINT64_C(-1)

#define HUFFMAN_FLAG   0x80
#define HUFFMAN_PREFIX 7

#define PREFIX_MASK(b) (uint8_t)((UINT16_C(1)<<(b))-1)

int hpack_encode_int(
		HPACK_INT_T i, size_t prefix_bits, uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint8_t prefix_mask;

	size_t _p;
	if (!produced) { produced = &_p; }
	*produced = 0;

	if (buff && n < 1) {
		return ERROR_OVERFLOW;
	}

	if (prefix_bits < 1 || prefix_bits > 8) {
		return ERROR_BAD_PREFIX;
	}

	prefix_mask = PREFIX_MASK(prefix_bits);
	if ((prefix & prefix_mask) != 0) {
		return ERROR_BAD_PREFIX;
	}

	if (i < prefix_mask) {
		if (buff) { *buff = (prefix | i); }
		*produced = 1;
		return NO_ERROR;
	} else {
		if (buff) { *buff = (prefix | prefix_mask); buff++; n--; }
		(*produced)++;
		i -= prefix_mask;
		while (i >= 0x80) {
			if (buff) {
				if (n < 1) {
					return ERROR_OVERFLOW;
				}
				*buff = ((i & 0x7F) | 0x80); buff++; n--;
			}
			(*produced)++;
			i >>= 7;
		}
	}
	if (buff) {
		if (n < 1) {
			return ERROR_OVERFLOW;
		}
		*buff = (uint8_t)i;
	}
	(*produced)++;
	return ERROR_NONE;
}

int hpack_decode_int(
		uint8_t *buff, size_t bytesize, size_t *consumed,
		size_t prefix_bits,
		HPACK_INT_T *i, uint8_t *prefix
) {
	uint8_t prefix_mask, b;
	uint64_t shift;
	HPACK_INT_T add;

	HPACK_INT_T _i; size_t _c;
	if (!i) { i = &_i; }
	if (!consumed) { consumed = &_c; }
	*consumed = 0;

	if (bytesize < 1) {
		return ERROR_TRUNCATED;
	}

	if (prefix_bits < 1 || prefix_bits > 8) {
		return ERROR_BAD_PREFIX;
	}

	prefix_mask = PREFIX_MASK(prefix_bits);
	b = *buff; buff++; (*consumed)++; bytesize--;

	*i = (b & prefix_mask);
	if (prefix) {
		*prefix = (b ^ *i);
	}

	if (*i >= prefix_mask) {
		shift = 1;
		while (1) {
			if (bytesize < 1) {
				return ERROR_TRUNCATED;
			}
			b = *buff; buff++; (*consumed)++; bytesize--;
			add = ((b & 0x7F) * shift);
			if ((MAX_HPACK_INT - (*i)) < add) {
				return ERROR_OVERFLOW;
			}
			*i += add;
			shift <<= 7;
			if ((b & 0x80) != 0x80) {
				break;
			}
		}
	}

	return ERROR_NONE;
}

inline int __hpack_encode_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
) {
	int error;
	size_t prod;

	error = hpack_encode_int(bytesize, HUFFMAN_PREFIX, prefix, buff, n, &prod);
	if (error) { return error; }

	if (n < prod + bytesize) {
		return ERROR_OVERFLOW;
	}

	memcpy(buff + prod, str, bytesize);
	if (produced) {
		*produced = prod + bytesize;
	}

	return ERROR_NONE;
}

int hpack_encode_raw_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	return __hpack_encode_str(str, bytesize, consumed, 0x00, buff, n, produced);
}

int hpack_encode_huff_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	return __hpack_encode_str(str, bytesize, consumed, HUFFMAN_FLAG, buff, n, produced);
}

int hpack_encode_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	int error;
	size_t length, prod, prod2;

	length = huffman_length(str, bytesize);
	if (length < bytesize) {
		/* huffman coding results in a shorter string */
		error = hpack_encode_int(length, HUFFMAN_PREFIX, HUFFMAN_FLAG, buff, n, &prod);
		if (error) { return error; }

		if (n < prod + bytesize) {
			return ERROR_OVERFLOW;
		}

		error = huffman_encode(str, bytesize, consumed, buff + prod, n - prod, &prod2);
		if (error) { return error; }

		*produced = prod + prod2;
		return ERROR_NONE;
	} else {
		/* no gain from huffman coding */
		return __hpack_encode_str(str, bytesize, consumed, 0x00, buff, n, produced);
	}
}

int hpack_decode_str(
	uint8_t *str, size_t bytesize, size_t *consumed,
	uint8_t *buff, size_t n, size_t *produced
) {
	size_t cons, cons2;
	HPACK_INT_T length;
	uint8_t prefix;
	int error;

	size_t c;
	if (!consumed) { consumed = &_c; }

	error = hpack_decode_int(str, bytesize, &cons, HUFFMAN_PREFIX, &length, &prefix);
	*consumed = cons;
	if (error) { return error; }

	if (prefix & HUFFMAN_FLAG) {
		/* huffman-decode remainder of string */
		error = huffman_decode(str + cons, bytesize - cons, &cons2, buff, n, produced);
		(*consumed) += cons2;
		if (error) { return error; }
		return ERROR_NONE;
	} else {
		/* string is unencoded, copy it as-is */
		if (length > n) {
			return ERROR_OVERFLOW;
		}
		memcpy(str + cons, buff, length);
		(*consumed) += length;
		*produced = length;
		return ERROR_NONE;
	}
}
