
#include <stddef.h>
#include <stdint.h>
#include "hpack_errors.h"

#define HPACK_INT_T uint64_t
#define MAX_HPACK_INT UINT64_C(-1)

#define PREFIX_MASK(b) (uint8_t)((UINT16_C(1)<<(b))-1)

int hpack_encoder_error;

size_t hpack_encode_int(
		HPACK_INT_T i, size_t prefix_bits, uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint8_t prefix_mask;

	size_t _p;
	if (!produced) { produced = &_p; }
	*produced = 0;

	hpack_encoder_error = ERROR_NONE;

	if (n < 1) {
		hpack_encoder_error = ERROR_OVERFLOW;
		return *produced;
	}

	if (prefix_bits < 1 || prefix_bits > 8) {
		hpack_encoder_error = ERROR_BAD_PREFIX;
		return *produced;
	}

	prefix_mask = PREFIX_MASK(prefix_bits);
	if ((prefix & prefix_mask) != 0) {
		hpack_encoder_error = ERROR_BAD_PREFIX;
		return *produced;
	}

	if (i < prefix_mask) {
		*buff = (prefix | i);
		return (*produced = 1);
	} else {
		*buff = (prefix | prefix_mask); buff++; (*produced)++; n--;
		i -= prefix_mask;
		while (i >= 0x80) {
			if (n < 1) {
				hpack_encoder_error = ERROR_OVERFLOW;
				return *produced;
			}
			*buff = ((i & 0x7F) | 0x80); buff++; (*produced)++; n--;
			i >>= 7;
		}
	}
	if (n < 1) {
		hpack_encoder_error = ERROR_OVERFLOW;
		return *produced;
	}
	*buff = (uint8_t)i; (*produced)++;
	return *produced;
}

int hpack_decoder_error;

HPACK_INT_T hpack_decode_int(
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

	hpack_decoder_error = ERROR_NONE;

	if (bytesize < 1) {
		hpack_encoder_error = ERROR_TRUNCATED;
		return MAX_HPACK_INT;
	}

	if (prefix_bits < 1 || prefix_bits > 8) {
		hpack_encoder_error = ERROR_BAD_PREFIX;
		return MAX_HPACK_INT;
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
				hpack_encoder_error = ERROR_TRUNCATED;
				return MAX_HPACK_INT;
			}
			b = *buff; buff++; (*consumed)++; bytesize--;
			add = ((b & 0x7F) * shift);
			if ((MAX_HPACK_INT - (*i)) < add) {
				hpack_encoder_error = ERROR_OVERFLOW;
				return MAX_HPACK_INT;
			}
			*i += add;
			shift <<= 7;
			if ((b & 0x80) != 0x80) {
				break;
			}
		}
	}

	return *i;
}

