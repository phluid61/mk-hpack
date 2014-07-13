typedef struct hpack_header {
	str key;
	str value;
} hpack_header;


inline
size_t hpack_header__size(hpack_header *h) {
	return h->key.length + h->value.length + 32;
}

inline
hpack_header *hpack_header__new(str *key, str *value) {
	hpack_header *ptr = (hpack_header*)calloc(1, sizeof(hpack_header));
	str__dup2(key, &(ptr->key));
	str__dup2(value, &(ptr->value));
	return ptr;
}

inline
hpack_header *hpack_header__new_shallow(str *key, str *value) {
	hpack_header *ptr = (hpack_header*)calloc(1, sizeof(hpack_header));
	str__dup2_shallow(key, &(ptr->key));
	str__dup2_shallow(value, &(ptr->value));
	return ptr;
}

inline
hpack_header *hpack_header__dup(hpack_header *this) {
	return hpack_header_new(&(this->key), &(this->value));
}

inline
void hpack_header__destroy(hpack_header *this) {
	str__destroy(&(this->key));
	str__destroy(&(this->value));
}
