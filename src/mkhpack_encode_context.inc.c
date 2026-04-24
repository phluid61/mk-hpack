
/*
 * HPACK encode context — RFC 7541 §6.
 *
 * Encodes a header list into a byte sequence, maintaining a dynamic
 * table across blocks within the same connection.
 *
 * Strategy:
 *   1. Exact match in static/dynamic table → indexed representation (§6.1)
 *   2. Name match → literal with incremental indexing (§6.2.1)
 *   3. No match → literal with incremental indexing (§6.2.1)
 *
 * Sensitive headers should use mkhpack_encode_header_never_indexed()
 * instead of the default path.
 */

typedef struct mkhpack_encode_context {
	mkhpack_header_table table;
} mkhpack_encode_context;

static void mkhpack_encode_context_init(mkhpack_encode_context *ctx, size_t max_table_size) {
	mkhpack_header_table_init(&ctx->table, max_table_size);
}

static void mkhpack_encode_context_destroy(mkhpack_encode_context *ctx) {
	mkhpack_header_table_destroy(&ctx->table);
}

/*
 * Encode an indexed header field (§6.1).
 * Wire format: 1xxxxxxx with 7-bit prefix.
 */
static int mkhpack_encode_indexed(
	size_t index,
	uint8_t *buff, size_t n, size_t *produced
) {
	return mkhpack_encode_int(
		(MKHPACK_INT_T)index, 7, 0x80,
		buff, n, produced
	);
}

/*
 * Encode a string (name or value) for a literal representation.
 * Uses mkhpack_encode_str which applies Huffman if shorter.
 */
static int mkhpack_encode_literal_str(
	const mkhpack_str *s,
	uint8_t *buff, size_t n, size_t *produced
) {
	return mkhpack_encode_str(s->ptr, s->length, NULL, buff, n, produced);
}

/*
 * Encode a literal header with incremental indexing (§6.2.1).
 * Wire format: 01xxxxxx with 6-bit prefix for name index.
 */
static int mkhpack_encode_literal_indexed(
	size_t name_index,
	const mkhpack_str *name, const mkhpack_str *value,
	uint8_t *buff, size_t n, size_t *produced
) {
	int err;
	size_t prod, total = 0;

	/* Prefix byte with name index */
	err = mkhpack_encode_int(
		(MKHPACK_INT_T)name_index, 6, 0x40,
		buff, n, &prod
	);
	if (err) return err;
	total += prod; buff += prod; n -= prod;

	/* If name_index == 0, encode the name */
	if (name_index == 0) {
		err = mkhpack_encode_literal_str(name, buff, n, &prod);
		if (err) return err;
		total += prod; buff += prod; n -= prod;
	}

	/* Encode the value */
	err = mkhpack_encode_literal_str(value, buff, n, &prod);
	if (err) return err;
	total += prod;

	*produced = total;
	return 0;
}

/*
 * Encode a literal header that must never be indexed (§6.2.3).
 * Wire format: 0001xxxx with 4-bit prefix for name index.
 */
static int mkhpack_encode_literal_never(
	size_t name_index,
	const mkhpack_str *name, const mkhpack_str *value,
	uint8_t *buff, size_t n, size_t *produced
) {
	int err;
	size_t prod, total = 0;

	err = mkhpack_encode_int(
		(MKHPACK_INT_T)name_index, 4, 0x10,
		buff, n, &prod
	);
	if (err) return err;
	total += prod; buff += prod; n -= prod;

	if (name_index == 0) {
		err = mkhpack_encode_literal_str(name, buff, n, &prod);
		if (err) return err;
		total += prod; buff += prod; n -= prod;
	}

	err = mkhpack_encode_literal_str(value, buff, n, &prod);
	if (err) return err;
	total += prod;

	*produced = total;
	return 0;
}

/*
 * Encode a dynamic table size update (§6.3).
 * Wire format: 001xxxxx with 5-bit prefix.
 */
static int mkhpack_encode_table_size_update(
	size_t new_max,
	uint8_t *buff, size_t n, size_t *produced
) {
	return mkhpack_encode_int(
		(MKHPACK_INT_T)new_max, 5, 0x20,
		buff, n, produced
	);
}

/*
 * Encode a single header.
 *
 * Searches both tables for a match and uses the most compact representation:
 *   - Exact match → indexed (§6.1)
 *   - Name match → literal with incremental indexing (§6.2.1)
 *   - No match → literal with incremental indexing, new name (§6.2.1)
 *
 * The header is added to the dynamic table.
 */
static int mkhpack_encode_header(
	mkhpack_encode_context *ctx,
	const mkhpack_str *name, const mkhpack_str *value,
	uint8_t *buff, size_t n, size_t *produced
) {
	int err, name_only = 0;
	size_t index;

	index = mkhpack_table_find(&ctx->table, name, value, &name_only);

	if (index > 0 && !name_only) {
		/* Exact match → indexed representation */
		err = mkhpack_encode_indexed(index, buff, n, produced);
		if (err) return err;
		return 0;
	}

	/* Literal with incremental indexing */
	err = mkhpack_encode_literal_indexed(
		name_only ? index : 0,
		name, value,
		buff, n, produced
	);
	if (err) return err;

	/* Add to dynamic table */
	mkhpack_header_table_add(&ctx->table, mkhpack_header_from(
		mkhpack_str_dup(name),
		mkhpack_str_dup(value)
	));

	return 0;
}

/*
 * Encode a sensitive header that must never be indexed (§6.2.3).
 *
 * Searches for a name match only; the header is NOT added to the
 * dynamic table.
 */
static int mkhpack_encode_header_never_indexed(
	mkhpack_encode_context *ctx,
	const mkhpack_str *name, const mkhpack_str *value,
	uint8_t *buff, size_t n, size_t *produced
) {
	int name_only = 0;
	size_t index;

	index = mkhpack_table_find(&ctx->table, name, value, &name_only);

	return mkhpack_encode_literal_never(
		(index > 0) ? index : 0,
		name, value,
		buff, n, produced
	);
}
