
hpack_header_table __STATIC_TABLE = (hpack_header_table){
	(hpack_header**)0,
	(size_t)0, (size_t)61,
	(size_t)2633, (size_t)2633,
	(size_t)61
};

void hpack_static_table__init() {
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(10,":authority"), &STR_C0); /* 1 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":method"), &STR_C(3,"GET"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":method"), &STR_C(4,"POST"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 5,":path"), &STR_C(1,"/"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 5,":path"), &STR_C(11,"/index.html"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":scheme"), &STR_C(4,"http"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":scheme"), &STR_C(5,"https"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"200"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"204"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"206"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"304"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"400"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"404"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,":status"), &STR_C(3,"500")); /* 14 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(14,"accept-charset"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(15,"accept-encoding"), &STR_C(13,"gzip, deflate"));
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(15,"accept-language"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(13,"accept-ranges"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 6,"accept"), &STR_C0); /* 19 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(27,"access-control-allow-origin"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 3,"age"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 5,"allow"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(13,"authorization"), &STR_C0); /* 23 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(13,"cache-control"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(19,"content-disposition"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(16,"content-encoding"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(16,"content-language"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(14,"content-length"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(16,"content-location"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(13,"content-range"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(12,"content-type"), &STR_C0); /* 31 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 6,"cookie"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 4,"date"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 4,"etag"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 6,"expect"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,"expires"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 4,"from"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 4,"host"), &STR_C0); /* 38 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 8,"if-match"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(17,"if-modified-since"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(13,"if-none-match"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 8,"if-range"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(19,"if-unmodified-since"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(13,"last-modified"), &STR_C0); /* 44 */
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 4,"link"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 8,"location"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(12,"max-forwards"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(18,"proxy-authenticate"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(19,"proxy-authorization"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 5,"range"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,"referer"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 7,"refresh"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(11,"retry-after"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 6,"server"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(10,"set-cookie"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(25,"strict-transport-security"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(17,"transfer-encoding"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(10,"user-agent"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 4,"vary"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C( 3,"via"), &STR_C0);
	hpack_header_table__add(&__STATIC_TABLE, &STR_C(16,"www-authenticate"), &STR_C0); /* 61 */
}

inline
hpack_header *hpack_static_table__get(size_t i) {
	hpack_header *e = hpack_header_table__get(&__STATIC_TABLE, i);
	if (e) e = hpack_header__dup(e);
	return e;
}

