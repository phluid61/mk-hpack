
typedef struct mkhpack_header {
	mkhpack_str name;
	mkhpack_str value;
} mkhpack_header;

/* RFC 7541 §4.1: entry size = name length + value length + 32 */
static inline size_t mkhpack_header_size(const mkhpack_header *h) {
	return h->name.length + h->value.length + 32;
}

/* Deep copy */
static inline mkhpack_header mkhpack_header_dup(const mkhpack_header *src) {
	mkhpack_header h;
	h.name = mkhpack_str_dup(&src->name);
	h.value = mkhpack_str_dup(&src->value);
	return h;
}

/* Construct from raw name/value, taking ownership of the str data */
static inline mkhpack_header mkhpack_header_from(mkhpack_str name, mkhpack_str value) {
	mkhpack_header h;
	h.name = name;
	h.value = value;
	return h;
}

static inline void mkhpack_header_destroy(mkhpack_header *h) {
	mkhpack_str_destroy(&h->name);
	mkhpack_str_destroy(&h->value);
}
