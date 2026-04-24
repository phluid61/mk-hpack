
#include <stddef.h>
#include <stdint.h>

#ifndef HAVE_MKHPACK
#define HAVE_MKHPACK 1

/** The type of integer that can be en/decoded by mkhpack_[en|de]code_int() */
#define MKHPACK_INT_T uint64_t


/**
 * Encode an integer as a sequence of bytes.
 *
 * If an error occurs, returns one of the following values:
 *
 * - 0: no error
 * - 1: overflow, +buff+ full
 * - 4: invalid +prefix_bits+, or invalid +prefix+
 *
 * @param MKHPACK_INT_T i       integer to encode
 * @param size_t    prefix_bits  bit-width of prefix (1 to 8 inclusive)
 * @param uint8_t   prefix    first byte, with bits before the prefix preloaded
 * @param uint8_t*  buff      pointer to start of output buffer
 * @param size_t    n         number of bytes allocated to output buffer
 * @param size_t*   produced  used to report number of bytes written
 * @return 
 */
int mkhpack_encode_int(
		MKHPACK_INT_T i, size_t prefix_bits, uint8_t prefix,
		uint8_t *buff, size_t n, size_t *produced
);


/**
 * Decode an integer from a sequence of bytes.
 *
 * If an error occurs, returns one of the following values:
 *
 * - 0: no error
 * - 1: overflow, the encoded integer is too large to represent in +i+
 * - 2: truncated, +buff+ exhausted mid-integer
 * - 4: invalid +prefix_bits+, or invalid +prefix+
 *
 * @param uint8_t*  buff      pointer to start of encoded bytes
 * @param size_t    bytesize  number of encoded bytes to decode
 * @param size_t*   consumed  used to report number of bytes read, may be NULL
 * @param size_t    prefix_bits  bit-width of prefix (1 to 8 inclusive)
 * @param MKHPACK_INT_T* i      pointer to the decoded integer
 * @param uint8_t*  prefix    first byte with only bits before the prefix set, may be NULL
 * @return 
 */
int mkhpack_decode_int(
		uint8_t *buff, size_t bytesize, size_t *consumed,
		size_t prefix_bits,
		MKHPACK_INT_T *i, uint8_t *prefix
);



/**
 * Huffman-encode a sequence of bytes.
 *
 * If an error occurs, returns one of the following values:
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int huffman_encode(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

/**
 * Calculate the length of a Huffman-encode a sequence of bytes.
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @return size_t number of encoded bytes
 */
size_t huffman_length(
		uint8_t *str, size_t bytesize
);

/**
 * Decode a sequence of Huffman-encoded bytes.
 *
 * If an error occurs, returns one of the following values:
 *
 * - 0: no error
 * - 1: overflow, output buffer full
 * - 2: truncated, input buffer exhausted mid-code
 * - 3: an invalid huffman code was encountered (EOS=256)
 *
 * @param uint8_t* huff     pointer to start of Huffman-encoded bytes
 * @param size_t   bytesize number of Huffman-encoded bytes to decode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int huffman_decode(
		uint8_t *huff, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);



/**
 * Encode a raw (not Huffman-encoded) string.
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int mkhpack_encode_raw_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

/**
 * Encode a Huffman-encoded string.
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int mkhpack_encode_huff_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);

/**
 * Encode a string. Applies Huffman encoding first, if that results
 * in a shorter output.
 *
 * @param uint8_t* str      pointer to start of bytes to encode
 * @param size_t   bytesize number of bytes to encode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int mkhpack_encode_str(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
);


/**
 * Decode a string.
 *
 * @param uint8_t* str      pointer to start of bytes to decode
 * @param size_t   bytesize number of bytes to decode
 * @param size_t * consumed used to report number of bytes read, may be NULL
 * @param uint8_t* buff     pointer to start of output buffer
 * @param size_t   n        number of bytes allocated to output buffer
 * @param size_t * produced used to report number of bytes written
 * @return 
 */
int mkhpack_decode_str(
	uint8_t *str, size_t bytesize, size_t *consumed,
	uint8_t *buff, size_t n, size_t *produced
);


/* ================================================================
 * Table layer — HPACK encode/decode contexts
 * ================================================================ */

/** Default dynamic table maximum size (RFC 7541 §4.2) */
#define MKHPACK_DEFAULT_TABLE_SIZE 4096

/* Opaque types */
typedef struct mkhpack_decode_context mkhpack_decode_context;
typedef struct mkhpack_encode_context mkhpack_encode_context;
typedef struct mkhpack_header_list   mkhpack_header_list;


/* --- Decode ---------------------------------------------------- */

/**
 * Create a new decode context with the given dynamic table size limit.
 *
 * @param size_t max_table_size  maximum dynamic table size in bytes
 * @return  a new context, or NULL on allocation failure
 */
mkhpack_decode_context *mkhpack_decode_context_new(size_t max_table_size);

/** Free a decode context and all associated resources. */
void mkhpack_decode_context_free(mkhpack_decode_context *ctx);

/**
 * Decode an HPACK header block.
 *
 * Processes [bytes, bytes+n) as a complete header block and returns
 * the decoded headers in *out.  The caller must free the list with
 * mkhpack_header_list_free().
 *
 * @param mkhpack_decode_context* ctx
 * @param uint8_t*  bytes     encoded header block
 * @param size_t    n         length of encoded data
 * @param size_t*   consumed  bytes consumed (may be NULL)
 * @param mkhpack_header_list** out  receives the decoded header list
 * @return 0 on success, or an error code
 */
int mkhpack_decode(
	mkhpack_decode_context *ctx,
	uint8_t *bytes, size_t n, size_t *consumed,
	mkhpack_header_list **out
);


/* --- Header list accessors ------------------------------------- */

/** Free a header list returned by mkhpack_decode(). */
void mkhpack_header_list_free(mkhpack_header_list *list);

/** Return the number of headers in the list. */
size_t mkhpack_header_list_count(const mkhpack_header_list *list);

/**
 * Get the name of the header at index i.
 *
 * @param list  header list
 * @param i     zero-based index
 * @param len   receives the name length (may be NULL)
 * @return pointer to the name bytes, or NULL if i is out of range
 */
const uint8_t *mkhpack_header_name(
	const mkhpack_header_list *list, size_t i, size_t *len
);

/**
 * Get the value of the header at index i.
 *
 * @param list  header list
 * @param i     zero-based index
 * @param len   receives the value length (may be NULL)
 * @return pointer to the value bytes, or NULL if i is out of range
 */
const uint8_t *mkhpack_header_value(
	const mkhpack_header_list *list, size_t i, size_t *len
);


/* --- Encode ---------------------------------------------------- */

/**
 * Create a new encode context with the given dynamic table size limit.
 *
 * @param size_t max_table_size  maximum dynamic table size in bytes
 * @return  a new context, or NULL on allocation failure
 */
mkhpack_encode_context *mkhpack_encode_context_new(size_t max_table_size);

/** Free an encode context and all associated resources. */
void mkhpack_encode_context_free(mkhpack_encode_context *ctx);

/**
 * Encode a single header field.
 *
 * Uses the most compact representation available (indexed, literal
 * with incremental indexing, or literal with new name) and updates
 * the dynamic table.
 *
 * @param ctx        encode context
 * @param name       header name bytes
 * @param name_len   header name length
 * @param value      header value bytes
 * @param value_len  header value length
 * @param buff       output buffer
 * @param n          output buffer size
 * @param produced   bytes written
 * @return 0 on success, or an error code
 */
int mkhpack_encode(
	mkhpack_encode_context *ctx,
	const uint8_t *name, size_t name_len,
	const uint8_t *value, size_t value_len,
	uint8_t *buff, size_t n, size_t *produced
);

/**
 * Encode a sensitive header that must never be indexed (§6.2.3).
 *
 * Same interface as mkhpack_encode(), but the header is NOT added to
 * the dynamic table.  Intermediaries that understand HPACK must not
 * add this header to their own tables.
 */
int mkhpack_encode_sensitive(
	mkhpack_encode_context *ctx,
	const uint8_t *name, size_t name_len,
	const uint8_t *value, size_t value_len,
	uint8_t *buff, size_t n, size_t *produced
);

/**
 * Emit a dynamic table size update (§6.3).
 *
 * Must be sent at the beginning of a header block if the encoder has
 * received a SETTINGS_HEADER_TABLE_SIZE change from the peer.
 *
 * @param ctx       encode context
 * @param new_max   new maximum table size
 * @param buff      output buffer
 * @param n         output buffer size
 * @param produced  bytes written
 * @return 0 on success, or an error code
 */
int mkhpack_encode_table_update(
	mkhpack_encode_context *ctx,
	size_t new_max,
	uint8_t *buff, size_t n, size_t *produced
);

#endif
