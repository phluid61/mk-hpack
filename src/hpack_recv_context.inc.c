
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

int
hpack_recv_context__recv_indexed(
	hpack_recv_context *this,
	uint8_t *bytes, size_t n, size_t *consumed,
	size_t prefix_bits,
	hpack_header **entry
) {
	int err;
	size_t cons;
	HPACK_INT_T idx;

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
		e = hpack_header_table__get(this->header_table, idx);
		name = str__dup(&(e->key));
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
		str__destroy(name); free(name);
	}
	return err;
}



void hpack_recv_context__recv(hpack_recv_context *this, uint8_t *bytes, size_t n) {
	int err;
	uint8_t b;
	size_t consumed;
	HPACK_INT_T i;
	hpack_header *e;
	while (n) {
		b = *bytes;
		if ((b & INDEXED_BIT) == INDEXED_BIT) {
			err = hpack_decode_int(bytes, n, &consumed, 7, &i, NULL);
			if (err) {/*...*/}
			if (i > hpack_header_table__length(this->header_table)) {
				e = hpack_static_table__get(i - hpack_header_table__length(this->header_table));
				/* if (!e) {...} */
				hpack_header_table__add_entry(this->header_table, e);
			} else {
				e = hpack_header_table__get(this->header_table, i);
			}
			hpack_header_table__add_entry(this->block, hpack_header__dup(e));
			bytes += consumed; n -= consumed;
		} else if ((b & LITERAL_INDEXED_BIT) == LITERAL_INDEXED_BIT) {
			err = hpack_recv_context__recv_indexed(this, bytes, n, &consumed, 6, &e);
			if (err) {/*...*/}
			hpack_header_table__add_entry(this->header_table, e);
			hpack_header_table__add_entry(this->block, hpack_header__dup(e));
			bytes += consumed; n -= consumed;
		} else if ((b & CONTEXT_UPDATE_BIT) == CONTEXT_UPDATE_BIT) {
			/* ##### */
		} else {
			/* "don't index" and "never index" are all the same to me */
			err = hpack_recv_context__recv_indexed(this, bytes, n, &consumed, 4, &e);
			if (err) {/*...*/}
			hpack_header_table__add_entry(this->block, e);
			bytes += consumed; n -= consumed;
		}
	}
}
