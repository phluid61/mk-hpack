
/*
 * Growable array of headers for decoded header blocks.
 */

typedef struct mkhpack_header_list {
	mkhpack_header *headers;
	size_t length;
	size_t capacity;
} mkhpack_header_list;

static void mkhpack_header_list_init(mkhpack_header_list *l) {
	l->headers = NULL;
	l->length = 0;
	l->capacity = 0;
}

static void mkhpack_header_list_destroy(mkhpack_header_list *l) {
	size_t i;
	for (i = 0; i < l->length; i++) {
		mkhpack_header_destroy(&l->headers[i]);
	}
	free(l->headers);
	l->headers = NULL;
	l->length = l->capacity = 0;
}

/* Append a header, taking ownership of its string data.  Returns 0 on success. */
static int mkhpack_header_list_append(mkhpack_header_list *l, mkhpack_header h) {
	if (l->length >= l->capacity) {
		size_t new_cap = l->capacity ? l->capacity * 2 : 16;
		mkhpack_header *tmp = (mkhpack_header *)realloc(
			l->headers, new_cap * sizeof(mkhpack_header)
		);
		if (!tmp) return ERROR_OVERFLOW;
		l->headers = tmp;
		l->capacity = new_cap;
	}
	l->headers[l->length++] = h;
	return 0;
}
