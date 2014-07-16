
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hpack_errors.h"
#include "hpack.h"

#define HPACK_INT_T uint64_t
#define MAX_HPACK_INT UINT64_C(-1)

#define HUFFMAN_FLAG   0x80
#define HUFFMAN_PREFIX 7

#define PREFIX_MASK(b) (uint8_t)((UINT16_C(1)<<(b))-1)

typedef struct hnode_t {
	uint32_t bits;
	uint8_t  size;
} hnode_t;

#include "huffman_codes.inc"

#define ZERO(tc) (uint16_t)((tc)>>16)
#define ONE(tc) (uint16_t)((tc)&0xFFFF)

#define IS_INT(x) (((x)&0x8000)==0x8000)
#define VALUE_OF(x) ((x)&0x7FFF)


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
		return ERROR_NONE;
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

	size_t _c;
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

int huffman_decode(
		uint8_t *huff, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint32_t tc = *HuffmanDecodes;
	uint16_t tmp;
	uint8_t byte, bc, mask;

	size_t _p, _c;
	if (!produced) { produced = &_p; }
	if (!consumed) { consumed = &_c; }
	*produced = *consumed = 0;

	if (bytesize < 1) {
		return ERROR_NONE;
	}

	if (buff && n < 1) {
		return ERROR_OVERFLOW;
	}

	while (bytesize > 0) {
		byte = *huff; huff++; (*consumed)++; bytesize--;
		bc = 0x80;   /* bit cursor */
		mask = 0x7F; /* padding mask */
		while (bc > 0) {
			if ((byte & bc) == bc) {
				tmp = ONE(tc);
			} else {
				tmp = ZERO(tc);
			}
			if (IS_INT(tmp)) {
				tmp = VALUE_OF(tmp);
				if (tmp > 0xFF) {
					return ERROR_EOS;
				} else {
					if (buff) {
						*buff = (uint8_t)(tmp); buff++;
						n--;
					}
					(*produced)++;
					if (bytesize < 1 && (byte & mask) == mask) {
						tc = 0;
						goto done;
					} else if (buff && n < 1) {
						return ERROR_OVERFLOW;
					} else {
						tc = *HuffmanDecodes;
					}
				}
			} else {
				/*tmp = VALUE_OF(tmp);*/
				/* FIXME: assert(tmp < 256) */
				tc = HuffmanDecodes[tmp];
			}
			bc >>= 1;
			mask >>= 1;
		}
	}
done:
	if (tc) {
		return ERROR_TRUNCATED;
	}
	return ERROR_NONE;
}

/* ^^^ Decoder | Encoder vvv */

int huffman_encode(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint8_t shift;
	uint64_t mask;
	uint64_t val;
	hnode_t hnode;

	uint64_t bitq = 0; /* a queue */
	uint8_t  bitn = 0; /* depth of the queue */

	size_t _p, _c;
	if (!produced) { produced = &_p; }
	if (!consumed) { consumed = &_c; }
	*produced = *consumed = 0;

	while (bytesize > 0) {
		if (buff && n < 1) {
			return ERROR_OVERFLOW;
		}
		hnode = HuffmanCodes[*str]; str++; (*consumed)++; bytesize--;
		bitq = (bitq << hnode.size) | hnode.bits; /* (max 33 bits wide) */
		bitn += hnode.size;
		/* canibalise the top bytes */
		while (bitn >= 8) {
			if (buff) {
				shift = bitn - 8;
				mask = UINT64_C(0xFF) << shift;
				val = (bitq & mask);
				*buff = (uint8_t)(val >> shift); buff++; n--;
				bitq ^= val;
			}
			(*produced)++;
			bitn -= 8;
		}
	}
	/* pad with EOS (incidentally all 1s) */
	if (bitn > 0) {
		if (buff) {
			shift = 8 - bitn;
			mask = (1 << shift) - 1;
			*buff = ((bitq << shift) | mask); buff++; n--;
		}
		(*produced)++;
	}
	return ERROR_NONE;
}

size_t huffman_length(
		uint8_t *str, size_t bytesize
) {
	size_t result = 7;
	while (bytesize > 0) {
		result += HuffmanCodes[*str].size; str++; bytesize--;
	}
	return (result / 8);
}



#include "str.inc.c"
#include "hpack_header.inc.c"
#include "hpack_header_table.inc.c"
#include "hpack_static_table.inc.c"

#define INDEXED_BIT          0x80
#define LITERAL_INDEXED_BIT  0x40
#define CONTEXT_UPDATE_BIT   0x20
#define LITERAL_NOINDEX_BIT  0x10

#define CONTEXT_UPDATE  0x20
#define FLUSH_REFSET  (CONTEXT_UPDATE|0x10)
#define RESIZE_TABLE  (CONTEXT_UPDATE|0x00)

#define hpack_gets(e, b,n,c, x,m,p, y, z) \
	e = hpack_decode_str(b,n,c,x,m,p); \
	if (e) { goto failed; } \
	(y) += *(c); \
	b += *(c); n -= *(c); \
	z = str__new2(x,*(p));

#include "hpack_recv_context.inc.c"

