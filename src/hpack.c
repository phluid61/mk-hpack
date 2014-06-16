
#include <stddef.h>
#include <stdint.h>
#include "hpack_errors.h"

#define HPACK_INT_T uint64_t
#define MAX_HPACK_INT UINT64_C(-1)

#define PREFIX_MASK(b) (uint8_t)((UINT16_C(1)<<(b))-1)

int hpack_encode_int(
		HPACK_INT_T i, size_t prefix_bits, uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint8_t prefix_mask;

	size_t _p;
	if (!produced) { produced = &_p; }
	*produced = 0;

	if (n < 1) {
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
		*buff = (prefix | i);
		*produced = 1;
		return NO_ERROR;
	} else {
		*buff = (prefix | prefix_mask); buff++; (*produced)++; n--;
		i -= prefix_mask;
		while (i >= 0x80) {
			if (n < 1) {
				return ERROR_OVERFLOW;
			}
			*buff = ((i & 0x7F) | 0x80); buff++; (*produced)++; n--;
			i >>= 7;
		}
	}
	if (n < 1) {
		return ERROR_OVERFLOW;
	}
	*buff = (uint8_t)i; (*produced)++;
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

