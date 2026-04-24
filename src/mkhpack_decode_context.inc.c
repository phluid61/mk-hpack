
/*
 * HPACK decode context — RFC 7541 §3, §4, §6.
 *
 * Processes an encoded header block into a header list, maintaining
 * a dynamic table across blocks within the same connection.
 */

/* Wire format bit patterns */
#define MKHPACK_INDEXED_BIT            0x80  /* 1xxxxxxx */
#define MKHPACK_LITERAL_INDEXED_BIT    0x40  /* 01xxxxxx */
#define MKHPACK_TABLE_UPDATE_BIT       0x20  /* 001xxxxx */
#define MKHPACK_LITERAL_NEVER_BIT      0x10  /* 0001xxxx */

typedef struct mkhpack_decode_context {
	mkhpack_header_table table;
} mkhpack_decode_context;

static void mkhpack_decode_context_init(mkhpack_decode_context *ctx, size_t max_table_size) {
	mkhpack_header_table_init(&ctx->table, max_table_size);
}

static void mkhpack_decode_context_destroy(mkhpack_decode_context *ctx) {
	mkhpack_header_table_destroy(&ctx->table);
}

/*
 * Decode a name or value string from the wire.
 * Allocates a new mkhpack_str with the decoded data.
 */
static int mkhpack_decode_context_read_str(
	uint8_t *bytes, size_t n, size_t *consumed, mkhpack_str *out
) {
	uint8_t buf[0x4000];
	size_t cons, prod;
	int err;

	err = mkhpack_decode_str(bytes, n, &cons, buf, sizeof(buf), &prod);
	if (err) return err;

	*out = mkhpack_str_from(buf, prod);
	*consumed = cons;
	return 0;
}

/*
 * Decode a literal header field (§6.2).
 * prefix_bits: 6 for incremental indexing, 4 for without/never indexing.
 */
static int mkhpack_decode_context_read_literal(
	mkhpack_decode_context *ctx,
	uint8_t *bytes, size_t n, size_t *consumed,
	size_t prefix_bits,
	mkhpack_header *out
) {
	int err;
	size_t cons;
	MKHPACK_INT_T name_idx;
	mkhpack_str name, value;

	*consumed = 0;

	/* Decode name index */
	err = mkhpack_decode_int(bytes, n, &cons, prefix_bits, &name_idx, NULL);
	if (err) return err;
	*consumed += cons;
	bytes += cons; n -= cons;

	if (name_idx == 0) {
		/* New name */
		err = mkhpack_decode_context_read_str(bytes, n, &cons, &name);
		if (err) return err;
		*consumed += cons;
		bytes += cons; n -= cons;
	} else {
		/* Indexed name */
		const mkhpack_header *ref = mkhpack_table_get(&ctx->table, (size_t)name_idx);
		if (!ref) return ERROR_BAD_INDEX;
		name = mkhpack_str_dup(&ref->name);
	}

	/* Decode value */
	err = mkhpack_decode_context_read_str(bytes, n, &cons, &value);
	if (err) {
		mkhpack_str_destroy(&name);
		return err;
	}
	*consumed += cons;

	*out = mkhpack_header_from(name, value);
	return 0;
}

/*
 * Decode a complete header block.
 *
 * Processes all representations in [bytes, bytes+n) and appends the
 * resulting headers to *list.  The caller must initialise the list
 * before calling (and destroy it when done).
 *
 * Returns 0 on success, or an error code.
 */
static int mkhpack_decode_block(
	mkhpack_decode_context *ctx,
	uint8_t *bytes, size_t n, size_t *consumed,
	mkhpack_header_list *list
) {
	int err;
	size_t total = 0;

	while (n > 0) {
		uint8_t b = *bytes;
		size_t cons = 0;

		if (b & MKHPACK_INDEXED_BIT) {
			/* §6.1 Indexed header field */
			MKHPACK_INT_T idx;
			const mkhpack_header *ref;

			err = mkhpack_decode_int(bytes, n, &cons, 7, &idx, NULL);
			if (err) goto fail;

			if (idx == 0) { err = ERROR_BAD_INDEX; goto fail; }

			ref = mkhpack_table_get(&ctx->table, (size_t)idx);
			if (!ref) { err = ERROR_BAD_INDEX; goto fail; }

			err = mkhpack_header_list_append(list, mkhpack_header_dup(ref));
			if (err) goto fail;

		} else if (b & MKHPACK_LITERAL_INDEXED_BIT) {
			/* §6.2.1 Literal with incremental indexing */
			mkhpack_header h;

			err = mkhpack_decode_context_read_literal(ctx, bytes, n, &cons, 6, &h);
			if (err) goto fail;

			/* Add a copy to the dynamic table */
			err = mkhpack_header_table_add(&ctx->table, mkhpack_header_dup(&h));
			if (err) { mkhpack_header_destroy(&h); goto fail; }

			err = mkhpack_header_list_append(list, h);
			if (err) goto fail;

		} else if (b & MKHPACK_TABLE_UPDATE_BIT) {
			/* §6.3 Dynamic table size update */
			MKHPACK_INT_T new_size;

			err = mkhpack_decode_int(bytes, n, &cons, 5, &new_size, NULL);
			if (err) goto fail;

			mkhpack_header_table_set_max_size(&ctx->table, (size_t)new_size);

		} else {
			/* §6.2.2 Without indexing (0000xxxx) or
			 * §6.2.3 Never indexed   (0001xxxx)
			 * — both use a 4-bit prefix and don't add to the table */
			mkhpack_header h;

			err = mkhpack_decode_context_read_literal(ctx, bytes, n, &cons, 4, &h);
			if (err) goto fail;

			err = mkhpack_header_list_append(list, h);
			if (err) goto fail;
		}

		bytes += cons; n -= cons; total += cons;
	}

	if (consumed) *consumed = total;
	return 0;

fail:
	if (consumed) *consumed = total;
	return err;
}
