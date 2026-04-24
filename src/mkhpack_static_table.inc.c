
/*
 * HPACK static table — RFC 7541 Appendix A.
 *
 * 61 pre-defined header entries, indexed 1–61.
 * Stored as a plain array; index i maps to static_table[i-1].
 */

#define MKHPACK_STATIC_TABLE_LENGTH 61

static const mkhpack_header mkhpack_static_table[MKHPACK_STATIC_TABLE_LENGTH] = {
	/* 1  */ { MKHPACK_STR_C(10, ":authority"),                  MKHPACK_STR_EMPTY },
	/* 2  */ { MKHPACK_STR_C( 7, ":method"),                     MKHPACK_STR_C( 3, "GET") },
	/* 3  */ { MKHPACK_STR_C( 7, ":method"),                     MKHPACK_STR_C( 4, "POST") },
	/* 4  */ { MKHPACK_STR_C( 5, ":path"),                       MKHPACK_STR_C( 1, "/") },
	/* 5  */ { MKHPACK_STR_C( 5, ":path"),                       MKHPACK_STR_C(11, "/index.html") },
	/* 6  */ { MKHPACK_STR_C( 7, ":scheme"),                     MKHPACK_STR_C( 4, "http") },
	/* 7  */ { MKHPACK_STR_C( 7, ":scheme"),                     MKHPACK_STR_C( 5, "https") },
	/* 8  */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "200") },
	/* 9  */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "204") },
	/* 10 */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "206") },
	/* 11 */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "304") },
	/* 12 */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "400") },
	/* 13 */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "404") },
	/* 14 */ { MKHPACK_STR_C( 7, ":status"),                     MKHPACK_STR_C( 3, "500") },
	/* 15 */ { MKHPACK_STR_C(14, "accept-charset"),              MKHPACK_STR_EMPTY },
	/* 16 */ { MKHPACK_STR_C(15, "accept-encoding"),             MKHPACK_STR_C(13, "gzip, deflate") },
	/* 17 */ { MKHPACK_STR_C(15, "accept-language"),             MKHPACK_STR_EMPTY },
	/* 18 */ { MKHPACK_STR_C(13, "accept-ranges"),               MKHPACK_STR_EMPTY },
	/* 19 */ { MKHPACK_STR_C( 6, "accept"),                      MKHPACK_STR_EMPTY },
	/* 20 */ { MKHPACK_STR_C(27, "access-control-allow-origin"), MKHPACK_STR_EMPTY },
	/* 21 */ { MKHPACK_STR_C( 3, "age"),                         MKHPACK_STR_EMPTY },
	/* 22 */ { MKHPACK_STR_C( 5, "allow"),                       MKHPACK_STR_EMPTY },
	/* 23 */ { MKHPACK_STR_C(13, "authorization"),               MKHPACK_STR_EMPTY },
	/* 24 */ { MKHPACK_STR_C(13, "cache-control"),               MKHPACK_STR_EMPTY },
	/* 25 */ { MKHPACK_STR_C(19, "content-disposition"),         MKHPACK_STR_EMPTY },
	/* 26 */ { MKHPACK_STR_C(16, "content-encoding"),            MKHPACK_STR_EMPTY },
	/* 27 */ { MKHPACK_STR_C(16, "content-language"),            MKHPACK_STR_EMPTY },
	/* 28 */ { MKHPACK_STR_C(14, "content-length"),              MKHPACK_STR_EMPTY },
	/* 29 */ { MKHPACK_STR_C(16, "content-location"),            MKHPACK_STR_EMPTY },
	/* 30 */ { MKHPACK_STR_C(13, "content-range"),               MKHPACK_STR_EMPTY },
	/* 31 */ { MKHPACK_STR_C(12, "content-type"),                MKHPACK_STR_EMPTY },
	/* 32 */ { MKHPACK_STR_C( 6, "cookie"),                      MKHPACK_STR_EMPTY },
	/* 33 */ { MKHPACK_STR_C( 4, "date"),                        MKHPACK_STR_EMPTY },
	/* 34 */ { MKHPACK_STR_C( 4, "etag"),                        MKHPACK_STR_EMPTY },
	/* 35 */ { MKHPACK_STR_C( 6, "expect"),                      MKHPACK_STR_EMPTY },
	/* 36 */ { MKHPACK_STR_C( 7, "expires"),                     MKHPACK_STR_EMPTY },
	/* 37 */ { MKHPACK_STR_C( 4, "from"),                        MKHPACK_STR_EMPTY },
	/* 38 */ { MKHPACK_STR_C( 4, "host"),                        MKHPACK_STR_EMPTY },
	/* 39 */ { MKHPACK_STR_C( 8, "if-match"),                    MKHPACK_STR_EMPTY },
	/* 40 */ { MKHPACK_STR_C(17, "if-modified-since"),           MKHPACK_STR_EMPTY },
	/* 41 */ { MKHPACK_STR_C(13, "if-none-match"),               MKHPACK_STR_EMPTY },
	/* 42 */ { MKHPACK_STR_C( 8, "if-range"),                    MKHPACK_STR_EMPTY },
	/* 43 */ { MKHPACK_STR_C(19, "if-unmodified-since"),         MKHPACK_STR_EMPTY },
	/* 44 */ { MKHPACK_STR_C(13, "last-modified"),               MKHPACK_STR_EMPTY },
	/* 45 */ { MKHPACK_STR_C( 4, "link"),                        MKHPACK_STR_EMPTY },
	/* 46 */ { MKHPACK_STR_C( 8, "location"),                    MKHPACK_STR_EMPTY },
	/* 47 */ { MKHPACK_STR_C(12, "max-forwards"),                MKHPACK_STR_EMPTY },
	/* 48 */ { MKHPACK_STR_C(18, "proxy-authenticate"),          MKHPACK_STR_EMPTY },
	/* 49 */ { MKHPACK_STR_C(19, "proxy-authorization"),         MKHPACK_STR_EMPTY },
	/* 50 */ { MKHPACK_STR_C( 5, "range"),                       MKHPACK_STR_EMPTY },
	/* 51 */ { MKHPACK_STR_C( 7, "referer"),                     MKHPACK_STR_EMPTY },
	/* 52 */ { MKHPACK_STR_C( 7, "refresh"),                     MKHPACK_STR_EMPTY },
	/* 53 */ { MKHPACK_STR_C(11, "retry-after"),                 MKHPACK_STR_EMPTY },
	/* 54 */ { MKHPACK_STR_C( 6, "server"),                      MKHPACK_STR_EMPTY },
	/* 55 */ { MKHPACK_STR_C(10, "set-cookie"),                  MKHPACK_STR_EMPTY },
	/* 56 */ { MKHPACK_STR_C(25, "strict-transport-security"),   MKHPACK_STR_EMPTY },
	/* 57 */ { MKHPACK_STR_C(17, "transfer-encoding"),           MKHPACK_STR_EMPTY },
	/* 58 */ { MKHPACK_STR_C(10, "user-agent"),                  MKHPACK_STR_EMPTY },
	/* 59 */ { MKHPACK_STR_C( 4, "vary"),                        MKHPACK_STR_EMPTY },
	/* 60 */ { MKHPACK_STR_C( 3, "via"),                         MKHPACK_STR_EMPTY },
	/* 61 */ { MKHPACK_STR_C(16, "www-authenticate"),            MKHPACK_STR_EMPTY },
};

/*
 * Look up a header by HPACK index (1-based, spanning static + dynamic table).
 *
 * Returns a const pointer to the entry, or NULL if the index is out of range.
 * For static entries (1–61), returns a pointer into the static table.
 * For dynamic entries (62+), returns a pointer into the dynamic table.
 */
static const mkhpack_header *mkhpack_table_get(
	const mkhpack_header_table *dynamic, size_t index
) {
	if (index == 0) return NULL;
	if (index <= MKHPACK_STATIC_TABLE_LENGTH) {
		return &mkhpack_static_table[index - 1];
	}
	return mkhpack_header_table_get(dynamic, index - MKHPACK_STATIC_TABLE_LENGTH - 1);
}

/*
 * Search both tables for a header.  Returns the HPACK index (1-based) of:
 *   - An exact match (name + value), or
 *   - A name-only match (sets *name_only = 1)
 * Returns 0 if no match is found.
 */
static size_t mkhpack_table_find(
	const mkhpack_header_table *dynamic,
	const mkhpack_str *name, const mkhpack_str *value,
	int *name_only
) {
	size_t name_match = 0;
	size_t i;

	/* Search static table */
	for (i = 0; i < MKHPACK_STATIC_TABLE_LENGTH; i++) {
		if (mkhpack_str_eq(name, &mkhpack_static_table[i].name)) {
			if (mkhpack_str_eq(value, &mkhpack_static_table[i].value)) {
				if (name_only) *name_only = 0;
				return i + 1;
			}
			if (name_match == 0) name_match = i + 1;
		}
	}

	/* Search dynamic table */
	for (i = 0; i < dynamic->length; i++) {
		const mkhpack_header *h = mkhpack_header_table_get(dynamic, i);
		if (mkhpack_str_eq(name, &h->name)) {
			if (mkhpack_str_eq(value, &h->value)) {
				if (name_only) *name_only = 0;
				return MKHPACK_STATIC_TABLE_LENGTH + 1 + i;
			}
			if (name_match == 0) {
				name_match = MKHPACK_STATIC_TABLE_LENGTH + 1 + i;
			}
		}
	}

	if (name_match > 0 && name_only) *name_only = 1;
	return name_match;
}
