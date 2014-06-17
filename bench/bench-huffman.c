#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "benchmark.h"
#include "../lib/hpack.h"

#define LLU(i) ((long long unsigned int)(i))
#define LF(f,d) ((long double)(f)/(long double)(d))

typedef struct str {
	size_t length;
	uint8_t *ptr;
} str;

/* suites of benchmarks to run */
#define SAMPLE_BY_PACKING 1
#define SAMPLE_HEX        0
#define SAMPLE_REAL_URIS  1
#define SAMPLE_BYTES      1

/* benchmark parameters for PACKING and HEX suites */
#define COUNT 100
#define LENGTH (1024*4)


#if SAMPLE_BY_PACKING
# define COUNT_QUADS 3
const uint8_t quad_soup[COUNT_QUADS] = {
	48, 49, 50,
};
# define COUNT_OCTAS 12
const uint8_t octa_soup[COUNT_OCTAS] = {
	59, 66, 67, 69, 73, 79, 80, 85, 88, 106, 107, 122,
};
# define COUNT_SHORTS 62
const uint8_t short_soup[COUNT_SHORTS] = {
	32, 37, 38, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
	57, 58, 59, 61, 65, 66, 67, 68, 69, 70, 71, 73, 77, 78, 79, 80,
	83, 84, 85, 88, 95, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
	108, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 121, 122,
};
# define COUNT_LONGS 194
const uint8_t long_soup[COUNT_LONGS] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	33, 34, 35, 36, 39, 40, 41, 42, 43, 60, 62, 63, 64, 72, 74, 75,
	76, 81, 82, 86, 87, 89, 90, 91, 92, 93, 94, 96, 113, 123, 124, 125,
	126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
	142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
	158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
	174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
	190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
	206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
	222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237,
	238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
	254, 255,
};
const str   quads[COUNT];
const str  hquads[COUNT];
const str   octas[COUNT];
const str  hoctas[COUNT];
const str  shorts[COUNT];
const str hshorts[COUNT];
const str   longs[COUNT];
const str  hlongs[COUNT];
#endif

#if SAMPLE_HEX
# define COUNT_HEX 22
# define COUNT_UCHEX 16
const uint8_t hex_soup[COUNT_HEX] = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F',
	'a','b','c','d','e','f',
};
# define COUNT_B64 64
const uint8_t b64_soup[COUNT_B64] = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J',
	'K','L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','a','b','c','d',
	'e','f','g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v','w','x',
	'y','z','+','/'
};
const str   hexes[COUNT];
const str  hhexes[COUNT];
const str   HEXes[COUNT];
const str  hHEXes[COUNT];
const str   b_64s[COUNT];
const str  hb_64s[COUNT];
#endif

/*
const uint64_t __uri_reserved[4] = {
	UINT64_C(0xac009fda00000000),
	UINT64_C(0x0000000028000001),
	UINT64_C(0x0000000000000000),
	UINT64_C(0x0000000000000000),
};
const uint64_t __uri_unreserved[4] = {
	UINT64_C(0x03ff600000000000),
	UINT64_C(0x47fffffe87fffffe),
	UINT64_C(0x0000000000000000),
	UINT64_C(0x0000000000000000),
};
const uint64_t __uri_forbidden[4] = {
	UINT64_C(0x50000025ffffffff),
	UINT64_C(0xb800000150000000),
	UINT64_C(0xffffffffffffffff),
	UINT64_C(0xffffffffffffffff),
};
inline int uri_reserved(  uint8_t b) {return !!(__uri_reserved  [b>>6]&(1<<(b&0x3F)));}
inline int uri_unreserved(uint8_t b) {return !!(__uri_unreserved[b>>6]&(1<<(b&0x3F)));}
inline int uri_forbidden( uint8_t b) {return !!(__uri_forbidden [b>>6]&(1<<(b&0x3F)));}
*/

#if SAMPLE_REAL_URIS
# define HOST_COUNT 8
const str  hosts[HOST_COUNT] = {
	(str){15, (uint8_t*)"www.example.com"},
	(str){14, (uint8_t*)"www.google.com"},
	(str){23, (uint8_t*)"fbstatic-a.akamaihd.net"},
	(str){15, (uint8_t*)"www.youtube.com"},
	(str){22, (uint8_t*)"www.library.qut.edu.au"},
	(str){18, (uint8_t*)"eprints.qut.edu.au"},
	(str){29, (uint8_t*)"digitalcollections.qut.edu.au"},
	(str){20, (uint8_t*)"matthew.kerwin.net.au"},
};
const str hhosts[HOST_COUNT];
# define HOST_LENGTH (15+14+23+15+22+18+29+20)
# define PATH_COUNT 8
const str  paths[PATH_COUNT] = {
	(str){1,  (uint8_t*)"/"},
	(str){151,(uint8_t*)"/xjs/_/js/k=xjs.s.en_US.LsU8tgs6eAk.O/m=sx,c,sb,cr,elog,jsa,r,hsm,pcc,csi/am=Ivw_FUZHUEACYBU/rt=j/d=1/sv=2/t=zcms/rs=AItRSTO0aOFXDxhZTQ28kL94SXVBLVU2gg"},
	(str){33, (uint8_t*)"/rsrc.php/v2/yw/r/Jegd7vMb5lV.png"},
	(str){20, (uint8_t*)"/watch?v=dQw4w9WgXcQ"},
	(str){29, (uint8_t*)"/images/new/corpsite-logo.png"},
	(str){100,(uint8_t*)"/secure/cgi/users/home?screen=User%3A%3AStaff%3A%3AEdit&userid=11721&_action_null=Administer+Account"},
	(str){48, (uint8_t*)"/style/images/home/promo-asia-pacific-images.jpg"},
	(str){29, (uint8_t*)"/blog/20131209_painting_sheds"},
};
const str hpaths[PATH_COUNT];
# define PATH_LENGTH (1+151+33+20+29+100+48+29)
#endif

#if SAMPLE_BYTES
# define N_BYTES 256
# define N_PACKED 612
uint8_t __total[N_BYTES+1] = {0};
uint8_t __htotal[N_PACKED+1] = {0};
const str total = (str){N_BYTES, __total};
const str htotal = (str){N_PACKED, __htotal};
void generate_total() {
	int i, j; char x[256] = {0};
	for (i = 0; i < 256; i++) {
		do {
			j = rand() % 256;
		} while (x[j]);
		x[j] = 1;
		total.ptr[i] = j;
	}
	huffman_encode(total.ptr, total.length, NULL, htotal.ptr, N_PACKED, NULL);
}
#endif

#define WORST_PACKED(l) ((l)*26)

#define n (size_t)(WORST_PACKED(LENGTH)*2)
const uint8_t buff[n];



uint64_t bench_encode_str(const str *in) {
	bench_start();
	huffman_encode(in->ptr, in->length, NULL, (uint8_t*)buff, n, NULL);
	return bench_end();
}
uint64_t bench_decode_str(const str *in) {
	bench_start();
	huffman_decode(in->ptr, in->length, NULL, (uint8_t*)buff, n, NULL);
	return bench_end();
}

uint64_t bench_encode_strs(const str *in, int i) {
	uint64_t ns = UINT64_C(0);
	while (i--) {
		ns += bench_encode_str(&(in[i]));
	}
	return ns;
}
uint64_t bench_decode_strs(const str *in, int i) {
	uint64_t ns = UINT64_C(0);
	while (i--) {
		ns += bench_decode_str(&(in[i]));
	}
	return ns;
}

void generate(uint8_t *soup, int num, str *out, str *hout) {
	int i = COUNT, j;
	while (i--) {
		out[i].ptr = (uint8_t*)calloc(LENGTH+1, sizeof(uint8_t));
		hout[i].ptr = (uint8_t*)calloc(WORST_PACKED(LENGTH)+1, sizeof(uint8_t));
		for (j = 0; j < LENGTH; j++) {
			out[i].ptr[j] = soup[rand() % num];
		}
		out[i].length = j;
		huffman_encode(out[i].ptr, out[i].length, NULL, hout[i].ptr, WORST_PACKED(LENGTH), &(hout[i].length));
		hout[i].ptr[hout[i].length] = 0;
	}
}
void generate_huff(str *in, int cnt, str *out) {
	while (cnt--) {
		out[cnt].ptr = (uint8_t*)calloc(WORST_PACKED(in[cnt].length)+1, sizeof(uint8_t));
		huffman_encode(in[cnt].ptr, in[cnt].length, NULL, out[cnt].ptr, WORST_PACKED(in[cnt].length), &(out[cnt].length));
		out[cnt].ptr[out[cnt].length] = 0;
	}
}

int main() {
	uint64_t x,y;

	srand(time(NULL));
#if SAMPLE_BY_PACKING
	generate((uint8_t*)quad_soup, COUNT_QUADS, (str*)quads, (str*)hquads);
	generate((uint8_t*)octa_soup, COUNT_OCTAS, (str*)octas, (str*)hoctas);
	generate((uint8_t*)short_soup,COUNT_SHORTS,(str*)shorts,(str*)hshorts);
	generate((uint8_t*)long_soup, COUNT_LONGS, (str*)longs, (str*)hlongs);
#endif
#if SAMPLE_HEX
	generate((uint8_t*)hex_soup,  COUNT_HEX,   (str*)hexes, (str*)hhexes);
	generate((uint8_t*)hex_soup,  COUNT_UCHEX, (str*)HEXes, (str*)hHEXes);
	generate((uint8_t*)b64_soup,  COUNT_B64,   (str*)b_64s, (str*)hb_64s);
#endif
#if SAMPLE_REAL_URIS
	generate_huff((str*)hosts, HOST_COUNT, (str*)hhosts);
	generate_huff((str*)paths, PATH_COUNT, (str*)hpaths);
#endif
	generate_total();

	init_bench();

#if SAMPLE_BY_PACKING || SAMPLE_HEX
	printf("\n%d strings per category\n%d bytes per string\n\n", COUNT, LENGTH);
#endif
#if SAMPLE_BY_PACKING
	printf(" bytes that encode to 4 bits\n");
	x=bench_encode_strs( quads ,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hquads ,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));

	printf(" bytes that encode to 8 bits\n");
	x=bench_encode_strs( octas ,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hoctas ,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));

	printf(" bytes that encode to <= 8 bits\n");
	x=bench_encode_strs( shorts,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hshorts,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));

	printf(" bytes that encode to >8 bits\n");
	x=bench_encode_strs( longs ,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hlongs ,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));
#endif
#if SAMPLE_HEX
	printf("hex characters\n");
	printf(" mix of upper- and lower-case\n");
	x=bench_encode_strs( hexes ,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hhexes ,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));
	printf(" upper-case only\n");
	x=bench_encode_strs( HEXes ,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hHEXes ,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));

	printf("base-64 characters\n");
	x=bench_encode_strs( b_64s ,COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	y=bench_decode_strs(hb_64s ,COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,COUNT*LENGTH));
	printf("           %10.2Lf\n", LF(y,x));
#endif
#if SAMPLE_REAL_URIS
	printf("\nTypical URLs\n");
	printf(" hosts\n");
	x=bench_encode_strs( hosts,HOST_COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,HOST_LENGTH));
	y=bench_decode_strs(hhosts,HOST_COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,HOST_LENGTH));
	printf("           %10.2Lf\n", LF(y,x));
	printf(" paths\n");
	x=bench_encode_strs( paths,PATH_COUNT);printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,PATH_LENGTH));
	y=bench_decode_strs(hpaths,PATH_COUNT);printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,PATH_LENGTH));
	printf("           %10.2Lf\n", LF(y,x));
#endif
	printf("\nRandom permutation of all 256 bytes\n\n");
	x=bench_encode_str(& total); printf("  Encode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,256.0));
	y=bench_decode_str(&htotal); printf("  Decode  : %10llu ns [%7.3Lf ns / byte]\n", LLU(y), LF(y,256.0));
	printf("           %10.2Lf\n", LF(y,x));

	return 0;
}

