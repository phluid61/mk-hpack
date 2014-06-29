#define hpack_header_table__size 512

typedef struct hpack_refset {
	hpack_header *refs[hpack_header_table__size];
	size_t count;
} hpack_refset;

inline
hpack_refset* hpack_refset__new() {
	return (hpack_refset*)calloc(1, sizeof(hpack_refset));
}

hpack_header* hpack_refset__find(hpack_refset *this, hpack_header *ref) {
	size_t i;
	for (i = 0; i < hpack_header_table__size; i++) {
		if (this->refs[i] == ref) {
			return ref;
		}
	}
	return (ref*)0;
}

hpack_header* hpack_refset__drop(hpack_refset *this, hpack_header *ref) {
	size_t i;
	for (i = 0; i < hpack_header_table__size; i++) {
		if (this->refs[i] == ref) {
			this->refs[i] = (hpack_header*)0;
			this->count --;
			return ref;
		}
	}
	return (hpack_header*)0;
}

hpack_header* hpack_refset__add(hpack_refset *this, hpack_header *ref) {
	size_t i, j=0;
	for (i = 0; i < hpack_header_table__size; i++) {
		if (this->refs[i] == ref) {
			return ref;
		} else if (this->refs[i] == (hpack_header*)0) {
			j = i;
		}
	}
	if (j) {
		this->refs[j] = ref;
		this->count ++;
		return ref;
	}
	return (hpack_header*)0;
}

void hpack_refset__flush(hpack_refset *this) {
	size_t i;
	for (i = 0; i < hpack_header_table__size; i++) {
		this->refs[i] = (hpack_header*)0;
	}
}

void hpack_refset__destroy(hpack_refset *this) {
	free(this->refs);
}
