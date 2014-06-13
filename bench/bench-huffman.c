#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "benchmark.h"
#include "../lib/huffman.h"

#define LLU(i) ((long long unsigned int)(i))
#define LF(f,d) ((long double)(f)/(d))

typedef struct str {
	size_t length;
	uint8_t *ptr;
} str;

#define COUNT_QUADS 3
const uint8_t quad_soup[COUNT_QUADS] = {
	48, 49, 50,
};
#define COUNT_OCTAS 12
const uint8_t octa_soup[COUNT_OCTAS] = {
	59, 66, 67, 69, 73, 79, 80, 85, 88, 106, 107, 122,
};
#define COUNT_SHORTS 62
const uint8_t short_soup[COUNT_SHORTS] = {
	32, 37, 38, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
	57, 58, 59, 61, 65, 66, 67, 68, 69, 70, 71, 73, 77, 78, 79, 80,
	83, 84, 85, 88, 95, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
	108, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 121, 122,
};
#define COUNT_LONGS 194
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

#define COUNT 100
#define LENGTH (1024*4)
const str quads[COUNT];
const str hquads[COUNT];
const str octas[COUNT];
const str hoctas[COUNT];
const str shorts[COUNT];
const str hshorts[COUNT];
const str longs[COUNT];
const str hlongs[COUNT];

#define N_BYTES 256
#define N_PACKED 612
uint8_t __total[N_BYTES+1] = {0};
uint8_t __htotal[N_PACKED+1] = {0};
const str total = (str){N_BYTES, __total};
const str htotal = (str){N_PACKED, __htotal};

#define WORST_PACKED (LENGTH*26)

#define n (size_t)(WORST_PACKED*2)
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

uint64_t bench_encode_strs(const str *in) {
	int i = COUNT;
	uint64_t ns = UINT64_C(0);
	while (i--) {
		ns += bench_encode_str(&(in[i]));
	}
	return ns;
}

uint64_t bench_decode_strs(const str *in) {
	int i = COUNT;
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
		hout[i].ptr = (uint8_t*)calloc(WORST_PACKED+1, sizeof(uint8_t));
		for (j = 0; j < LENGTH; j++) {
			out[i].ptr[j] = soup[rand() % num];
		}
		out[i].length = j;
		hout[i].length = huffman_encode(out[i].ptr, out[i].length, NULL, hout[i].ptr, WORST_PACKED, NULL);
	}
}
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

int main() {
	uint64_t x; int i = COUNT;

	srand(time(NULL));
	generate((uint8_t*)quad_soup, COUNT_QUADS, (str*)quads, (str*)hquads);
	generate((uint8_t*)octa_soup, COUNT_OCTAS, (str*)octas, (str*)hoctas);
	generate((uint8_t*)short_soup,COUNT_SHORTS,(str*)shorts,(str*)hshorts);
	generate((uint8_t*)long_soup, COUNT_LONGS, (str*)longs, (str*)hlongs);
	generate_total();

	init_bench();

	printf("\nquads = bytes that encode to 4 bits\noctas = bytes that encode to 8 bits\nshorts = bytes that encode to <= 8 bits\nlongs = bytes that encode to >8 bits\n\n");
	printf("%d strings per category\n%d bytes per string\n\n", COUNT, LENGTH);
	x=bench_encode_strs( quads );printf(" Encode quads  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_encode_strs( octas );printf(" Encode octas  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_encode_strs( shorts);printf(" Encode shorts : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_encode_strs( longs );printf(" Encode longs  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_decode_strs(hquads );printf(" Decode quads  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_decode_strs(hoctas );printf(" Decode octas  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_decode_strs(hshorts);printf(" Decode shorts : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	x=bench_decode_strs(hlongs );printf(" Decode longs  : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,COUNT*LENGTH));
	printf("\nRandom permutation of all 256 bytes\n\n");
	x=bench_encode_str(& total);printf(" Encode all bytes : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,256.0));
	x=bench_decode_str(&htotal);printf(" Decode all bytes : %10llu ns [%7.3Lf ns / byte]\n", LLU(x), LF(x,256.0));

	while (i--) {
		free(quads[i].ptr); free(hquads[i].ptr);
		free(octas[i].ptr); free(hoctas[i].ptr);
		free(shorts[i].ptr);free(hshorts[i].ptr);
		free(longs[i].ptr); free(hlongs[i].ptr);
	}
	return 0;
}

