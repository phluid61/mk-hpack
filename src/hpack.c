
#include <stddef.h>
#include <stdint.h>
#include <string.h>
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

#define INDEXED_BIT          0x80
#define LITERAL_INDEXED_BIT  0x40
#define CONTEXT_UPDATE_BIT   0x20
#define LITERAL_NOINDEX_BIT  0x10

#define CONTEXT_UPDATE  0x20
#define FLUSH_REFSET  (CONTEXT_UPDATE|0x10)
#define RESIZE_TABLE  (CONTEXT_UPDATE|0x00)


typedef struct hpack_recv_context {
	hpack_header_table *header_table;
	hpack_header_table *block;
} hpack_recv_context;

inline
hpack_recv_context *hpack_recv_context__new(size_t max) {
	hpack_recv_context *ptr = (hpack_recv_context*)calloc(1, sizeof(hpack_recv_context));
	ptr->header_table = hpack_header_table__new(max);
	ptr->block = hpack_header_table__new((size_t)-1);
	return ptr;
}

inline
void hpack_recv_context__destroy(hpack_recv_context *this) {
	hpack_header_table__destroy(this->header_table); free(this->header_table);
	hpack_header_table__destroy(this->block); free(this->block);
}

#define hpack_gets(e, b,n,c, x,m,p, y, z) \
	e = hpack_decode_str(b,n,c,x,m,p); \
	if (e) { goto failed; } \
	(y) += c; \
	b += c; n -= c; \
	z = str__new2(x,p);

int
hpack_recv_context__recv_indexed(
	hpack_recv_context *this,
	uint8_t *bytes, size_t n, size_t *consumed,
	size_t prefix_bits,
	hpack_header **entry
) {
	int err;
	size_t cons;
	HPACK_INT_T i;

	uint8_t buff[0x4000];
	size_t prod;
	str *name=0, *value;

	hpack_header *e;

	size_t _c;
	if (!consumed) { consumed = &_c; }
	*consumed = 0;

	err = hpack_decode_int(
		bytes, n, &cons,
		prefix_bits,
		&idx, NULL
	);
	if (err) { return err; }
	(*consumed) += cons;
	bytes += cons; n -= cons;

	if (idx == 0) {
		hpack_gets(
			err,
			bytes, n, &cons,
			buff, 256, &prod,
			*consumed,
			name
		);
	} else {
		e = hpack_header_table__get(this, idx);
		name = str_dup(&(e->key));
	}

	hpack_gets(
		err,
		bytes, n, &cons,
		buff, 0x4000, &prod,
		*consumed,
		value
	);

	*entry = hpack_header__new_shallow(name, value);
	return 0;

failed:
	if (name) {
		str_destroy(name); free(name);
	}
	return err;
}

hpack_header __STATIC_TABLE_TABLE[61] = {
	(hpack_header){STR_C(10,":authority"), STR_C0}, /* 1 */
	(hpack_header){STR_C( 7,":method"), STR_C(3,"GET")},
	(hpack_header){STR_C( 7,":method"), STR_C(4,"POST")},
	(hpack_header){STR_C( 5,":path"), STR_C(1,"/")},
	(hpack_header){STR_C( 5,":path"), STR_C(11,"/index.html")},
	(hpack_header){STR_C( 7,":scheme"), STR_C(4,"http")},
	(hpack_header){STR_C( 7,":scheme"), STR_C(5,"https")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"200")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"204")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"206")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"304")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"400")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"404")},
	(hpack_header){STR_C( 7,":status"), STR_C(3,"500")}, /* 14 */
	(hpack_header){STR_C(14,"accept-charset"), STR_C0},
	(hpack_header){STR_C(15,"accept-encoding"), STR_C(13,"gzip, deflate")},
	(hpack_header){STR_C(15,"accept-language"), STR_C0},
	(hpack_header){STR_C(13,"accept-ranges"), STR_C0},
	(hpack_header){STR_C( 6,"accept"), STR_C0}, /* 19 */
	(hpack_header){STR_C(27,"access-control-allow-origin"), STR_C0},
	(hpack_header){STR_C( 3,"age"), STR_C0},
	(hpack_header){STR_C( 5,"allow"), STR_C0},
	(hpack_header){STR_C(13,"authorization"), STR_C0}, /* 23 */
	(hpack_header){STR_C(13,"cache-control"), STR_C0},
	(hpack_header){STR_C(19,"content-disposition"), STR_C0},
	(hpack_header){STR_C(16,"content-encoding"), STR_C0},
	(hpack_header){STR_C(16,"content-language"), STR_C0},
	(hpack_header){STR_C(14,"content-length"), STR_C0},
	(hpack_header){STR_C(16,"content-location"), STR_C0},
	(hpack_header){STR_C(13,"content-range"), STR_C0},
	(hpack_header){STR_C(12,"content-type"), STR_C0}, /* 31 */
	(hpack_header){STR_C( 6,"cookie"), STR_C0},
	(hpack_header){STR_C( 4,"date"), STR_C0},
	(hpack_header){STR_C( 4,"etag"), STR_C0},
	(hpack_header){STR_C( 6,"expect"), STR_C0},
	(hpack_header){STR_C( 7,"expires"), STR_C0},
	(hpack_header){STR_C( 4,"from"), STR_C0},
	(hpack_header){STR_C( 4,"host"), STR_C0}, /* 38 */
	(hpack_header){STR_C( 8,"if-match"), STR_C0},
	(hpack_header){STR_C(17,"if-modified-since"), STR_C0},
	(hpack_header){STR_C(13,"if-none-match"), STR_C0},
	(hpack_header){STR_C( 8,"if-range"), STR_C0},
	(hpack_header){STR_C(19,"if-unmodified-since"), STR_C0},
	(hpack_header){STR_C(13,"last-modified"), STR_C0}, /* 44 */
	(hpack_header){STR_C( 4,"link"), STR_C0},
	(hpack_header){STR_C( 8,"location"), STR_C0},
	(hpack_header){STR_C(12,"max-forwards"), STR_C0},
	(hpack_header){STR_C(18,"proxy-authenticate"), STR_C0},
	(hpack_header){STR_C(19,"proxy-authorization"), STR_C0},
	(hpack_header){STR_C( 5,"range"), STR_C0},
	(hpack_header){STR_C( 7,"referer"), STR_C0},
	(hpack_header){STR_C( 7,"refresh"), STR_C0},
	(hpack_header){STR_C(11,"retry-after"), STR_C0},
	(hpack_header){STR_C( 6,"server"), STR_C0},
	(hpack_header){STR_C(10,"set-cookie"), STR_C0},
	(hpack_header){STR_C(25,"strict-transport-security"), STR_C0},
	(hpack_header){STR_C(17,"transfer-encoding"), STR_C0},
	(hpack_header){STR_C(10,"user-agent"), STR_C0},
	(hpack_header){STR_C( 4,"vary"), STR_C0},
	(hpack_header){STR_C( 3,"via"), STR_C0},
	(hpack_header){STR_C(16,"www-authenticate"), STR_C0}, /* 61 */
};
hpack_header_table __STATIC_TABLE = (hpack_header_table){
	&__STATIC_TABLE_TABLE,
	(size_t)0, (size_t)61,
	(size_t)2633, (size_t)2633,
	(size_t)61
};

inline
hpack_header *hpack_static_table__get(size_t i) {
	hpack_header *e = hpack_header_table__get(&__STATIC_TABLE, i);
	if (e) e = hpack_header__dup(e);
	return e;
}

void hpack_recv_context__recv(hpack_recv_context *this, uint8_t *bytes, size_t n) {
	int err;
	uint8_t b;
	size_t consumed;
	HPACK_INT_T i;
	hpack_header *e;
	while (n) {
		b = *bytes;
		if (b & INDEXED_BIT == INDEXED_BIT) {
			err = hpack_decode_int(bytes, n, &consumed, 7, &i, NULL);
			/* if (err) {...} */
			hpack_recv_context__recv_index(this, i);
			if (i > hpack_header_table__length(this)) {
				e = hpack_static_table__get(i - hpack_header_table__length(this));
				/* if (!e) {...} */
				hpack_header_table__add_entry(this->header_table, e)
			} else {
				e = hpack_header_table__get(this->header_table, i);
			}
			hpack_header_table__add_entry(this->block, hpack_header__dup(e));
			bytes += consumed; n -= consumed;
		} else if (b & LITERAL_INDEXED_BIT == LITERAL_INDEXED_BIT) {
			err = hpack_recv_context__recv_indexed(this, bytes, n, &consumed, 6, &e);
			/* if (err) {...} */
			hpack_header_table__add_entry(this->header_table, e);
			hpack_header_table__add_entry(this->block, hpack_header__dup(e));
			bytes += consumed; n -= consumed;
		} else if (b & CONTEXT_UPDATE_BIT == CONTEXT_UPDATE_BIT) {
			/* ##### */
		} else {
			/* "don't index" and "never index" are all the same to me */
			err = hpack_recv_context__recv_indexed(this, bytes, n, &consumed, 4, &e);
			/* if (err) {...} */
			hpack_header_table__add_entry(this->block, e);
			bytes += consumed; n -= consumed;
		}
	}
}
