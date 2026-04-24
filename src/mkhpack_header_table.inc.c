
/*
 * Dynamic header table — circular buffer with eviction.
 *
 * Entries are indexed 0..length-1 where 0 is the most recently added.
 * Eviction removes from the oldest end when total size exceeds max_size.
 */

typedef struct mkhpack_header_table {
	mkhpack_header *entries;
	size_t capacity;
	size_t head;     /* slot of newest entry */
	size_t length;   /* number of entries */
	size_t size;     /* current size in bytes (RFC 7541 §4.1) */
	size_t max_size; /* maximum size in bytes */
} mkhpack_header_table;

static void mkhpack_header_table_init(mkhpack_header_table *t, size_t max_size) {
	/* max entries = max_size / 32, since each entry is at least 32 bytes */
	size_t cap = max_size / 32;
	if (cap < 16) cap = 16;

	t->entries = (mkhpack_header *)calloc(cap, sizeof(mkhpack_header));
	t->capacity = cap;
	t->head = 0;
	t->length = 0;
	t->size = 0;
	t->max_size = max_size;
}

static void mkhpack_header_table_destroy(mkhpack_header_table *t) {
	size_t i;
	for (i = 0; i < t->length; i++) {
		size_t idx = (t->head + t->capacity - i) % t->capacity;
		mkhpack_header_destroy(&t->entries[idx]);
	}
	free(t->entries);
	t->entries = NULL;
	t->capacity = t->length = t->size = 0;
}

/* Evict oldest entries until size <= target */
static void mkhpack_header_table_evict(mkhpack_header_table *t, size_t target) {
	while (t->length > 0 && t->size > target) {
		/* oldest entry is at (head - length + 1) mod capacity */
		size_t oldest = (t->head + t->capacity - t->length + 1) % t->capacity;
		t->size -= mkhpack_header_size(&t->entries[oldest]);
		mkhpack_header_destroy(&t->entries[oldest]);
		t->length--;
	}
}

static void mkhpack_header_table_set_max_size(mkhpack_header_table *t, size_t max_size) {
	t->max_size = max_size;
	mkhpack_header_table_evict(t, max_size);
}

/*
 * Add an entry to the table, taking ownership of the header's string data.
 * Evicts oldest entries as needed to stay within max_size.
 * Returns 0 on success, or ERROR_TABLE_FULL if the entry is too large
 * to fit even in an empty table.
 */
static int mkhpack_header_table_add(mkhpack_header_table *t, mkhpack_header entry) {
	size_t entry_size = mkhpack_header_size(&entry);

	if (entry_size > t->max_size) {
		/* RFC 7541 §4.4: entry larger than max causes full eviction */
		mkhpack_header_table_evict(t, 0);
		mkhpack_header_destroy(&entry);
		return 0;
	}

	mkhpack_header_table_evict(t, t->max_size - entry_size);

	/* Advance head */
	if (t->length > 0) {
		t->head = (t->head + 1) % t->capacity;
	}
	t->entries[t->head] = entry;
	t->length++;
	t->size += entry_size;

	return 0;
}

/*
 * Get entry by index (0 = most recently added).
 * Returns a const pointer into the table; the caller must not hold it
 * across mutations.  Returns NULL if index is out of range.
 */
static const mkhpack_header *mkhpack_header_table_get(
	const mkhpack_header_table *t, size_t i
) {
	if (i >= t->length) return NULL;
	size_t idx = (t->head + t->capacity - i) % t->capacity;
	return &t->entries[idx];
}
