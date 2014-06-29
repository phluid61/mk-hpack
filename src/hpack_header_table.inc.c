#define hpack_header_table__size 128

typedef struct hpack_header_table {
	hpack_header **table;
	size_t        table_head;
	size_t        table_tail;

	size_t        size;
	size_t        max;
	hpack_refset *refset;
} hpack_header_table;

inline
hpack_header_table *hpack_header_table__new(size_t max, hpack_refset *refset) {
	hpack_header_table *ptr = (hpack_header_table*)calloc(1, sizeof(hpack_header_table));
	ptr->table = (hpack_header**)calloc(hpack_header_table__size, sizeof(hpack_header*));
	ptr->table_head = 0;
	ptr->table_tail = 0;
	ptr->size = 0;
	ptr->max = max;
	ptr->refset = refset;
	return ptr;
}

inline
size_t hpack_header_table__set_max(hpack_header_table *this, size_t max) {
	this->max = max;
	hpack_header_table__evict(this, max);
	return this->max;
}

inline
size_t hpack_header_table__add(hpack_header_table *this, str *name, str *value) {
	return hpack_header_table__add_entry(this, hpack_header__new(name, value));
}

size_t hpack_header_table__add_entry(hpack_header_table *this, hpack_header *entry) {
	size_t i, ttl_bytes, nil_bytes;
	size_t prev;
	size_t e_size = hpack_header__size(entry);
	hpack_header_table__evict(this, this->max - e_size);
	if (e_size < this->max) {
		if (table_head == 0) {
			prev = hpack_header_table__size - 1;
		} else {
			prev = this->table_head - 1;
		}
		/* if table is full ... */
		if (prev == table_tail) {
			/* TODO: grow? fail better? */
			return ~0;
		}
		/* insert at the left */
		this->table[prev] = entry;
		this->table_head = prev;
		this->size += e_size;
		return 1;
	} else {
		return 0;
	}
}


void hpack_header_table__evict(hpack_header_table *this, size_t max) {
	size_t prev;
	hpack_header *e;
	while (this->size > max) {
		e = this->table[this->table_tail];
		if (this->table_tail == 0) {
			this->table_tail = hpack_header_table__size - 1;
		} else {
			this->table_tail --;
		}
		this->size -= hpack_header__size(e);
		if (hpack_refset__drop(this->refset, e)) {
			hpack_header__destroy(e);
			free(e);
		}
	}
}
