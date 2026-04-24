
typedef struct mkhpack_str {
	size_t length;
	uint8_t *ptr;
} mkhpack_str;

#define MKHPACK_STR_C(l,p) ((mkhpack_str){(size_t)(l),(uint8_t*)(p)})
#define MKHPACK_STR_EMPTY  ((mkhpack_str){(size_t)0,(uint8_t*)0})

/* Deep copy: allocates new buffer and copies data */
static inline mkhpack_str mkhpack_str_dup(const mkhpack_str *src) {
	mkhpack_str s;
	s.length = src->length;
	if (src->ptr && src->length > 0) {
		s.ptr = (uint8_t *)malloc(src->length);
		memcpy(s.ptr, src->ptr, src->length);
	} else {
		s.ptr = NULL;
	}
	return s;
}

/* Construct from raw bytes: allocates new buffer */
static inline mkhpack_str mkhpack_str_from(const uint8_t *bytes, size_t n) {
	mkhpack_str s;
	s.length = n;
	if (bytes && n > 0) {
		s.ptr = (uint8_t *)malloc(n);
		memcpy(s.ptr, bytes, n);
	} else {
		s.ptr = NULL;
	}
	return s;
}

/* Free owned data */
static inline void mkhpack_str_destroy(mkhpack_str *s) {
	free(s->ptr);
	s->ptr = NULL;
	s->length = 0;
}

/* Compare two strings for equality */
static inline int mkhpack_str_eq(const mkhpack_str *a, const mkhpack_str *b) {
	if (a->length != b->length) return 0;
	if (a->length == 0) return 1;
	return memcmp(a->ptr, b->ptr, a->length) == 0;
}
