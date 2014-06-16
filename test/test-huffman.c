
#include <stdio.h>
#include <stdlib.h>

#include "../lib/huffman.h"

typedef struct str {
	size_t length;
	uint8_t *ptr;
} str;

void dump(uint8_t* buff, size_t n, char token, char literal) {
	size_t j;
	printf(" %c ", token);
	for (j = 0; j < n; j++) {
		printf(" %02x", buff[j]);
	}
	printf(".\n");

#ifndef NO_LITERALS
	if (literal) {
		printf("    \"");
		for (j = 0; j < n; j++) {
			printf("%c", buff[j]);
		}
		printf("\"\n");
	}
#endif
}

int test_decode() {
	const size_t good_n = 8;
	const str good_in[] = {
		((str){0, (uint8_t*)""}),
		((str){1, (uint8_t*)"\xEC"}),
		((str){1, (uint8_t*)"G"}),
		((str){2, (uint8_t*)"B?"}),
		((str){2, (uint8_t*)"\x10\x20"}),
		((str){4, (uint8_t*)"\xFF\xFF\xFF\xFF"}),
		((str){12, (uint8_t*)"\xE7\xCF\x9B\xEB\xE8\x9B\x6F\xB1\x6F\xA9\xB6\xFF"}),
		((str){17, (uint8_t*)"\x3B\xFC\xD7\x65\x99\xBD\xAE\x6F\x3B\x8F\x5B\x71\x76\x6B\x56\xE4\xFF"}),
	};
	const str good_out[] = {
		((str){0, (uint8_t*)""}),
		((str){1, (uint8_t*)";"}),
		((str){1, (uint8_t*)"3"}),
		((str){2, (uint8_t*)"33"}),
		((str){4, (uint8_t*)"1020"}),
		((str){1, (uint8_t*)"\xA3"}),
		((str){15, (uint8_t*)"www.example.com"}),
		((str){22, (uint8_t*)"/.well-known/host-meta"}),
	};

	const size_t bad_n = 2;
	const str bad_in[] = {
		((str){1, (uint8_t*)"\xE0"}), /* \x1E would be 'f', but wrong padding */
		((str){4, (uint8_t*)"\xFF\xFF\xEE\x7F"}), /* valid encoding of EOS */
	};

	int huffman_decoder_error;

	const size_t n = 32;
	uint8_t buff[33]; /*n+1*/

	char match; int retval=0;
	size_t i,j;
	size_t length;

	printf("**\n** DECODER\n**\n\n");

	for (i = 0; i < good_n; i++) {
		printf("Test %d:\n", (int)i);
		dump(good_in[i].ptr, good_in[i].length, '<', 0);

		/* aha! sending uninitialised buff to my function! */
		huffman_decoder_error = huffman_decode(good_in[i].ptr, good_in[i].length, NULL, buff, n, &length);

		if (huffman_decoder_error) {
			printf(" * error: %d\n", huffman_decoder_error);
		}

		if (length == good_out[i].length) {
			match = 1;
			for (j = 0; j < length; j++) {
				if (buff[j] != good_out[i].ptr[j]) {
					printf(" * mismatch at byte %d: got %02x, expected %02x\n", (int)j, buff[j], good_out[i].ptr[j]);
					match = 0;
					break;
				}
			}
		} else {
			match = 0;
			printf(" * returned %d, expected %d\n", (int)length, (int)good_out[i].length);
		}

		/*if (!match) {*/
			dump(buff, length, '>', 1);
			dump(good_out[i].ptr, good_out[i].length, '~', 1);
		/*}*/
		printf("\n");

		retval += (!(match)||!!huffman_decoder_error);
	}

	for (i = 0; i < bad_n; i++) {
		printf("Test %d:\n", (int)(good_n+i));
		dump(bad_in[i].ptr, bad_in[i].length, '<', 0);

		/* aha! sending uninitialised buff to my function! */
		huffman_decoder_error = huffman_decode(bad_in[i].ptr, bad_in[i].length, NULL, buff, n, &length);

		if (huffman_decoder_error) {
			printf(" * error: %d\n", huffman_decoder_error);
		}

		dump(buff, length, '>', 1);
		printf("\n");

		retval += (!huffman_decoder_error);
	}

	return retval;
}

int test_encode() {
	const size_t in_n = 8;
	const str in[] = {
		((str){0, (uint8_t*)""}),
		((str){1, (uint8_t*)";"}),
		((str){1, (uint8_t*)"3"}),
		((str){2, (uint8_t*)"33"}),
		((str){4, (uint8_t*)"1020"}),
		((str){1, (uint8_t*)"\xA3"}),
		((str){15, (uint8_t*)"www.example.com"}),
		((str){22, (uint8_t*)"/.well-known/host-meta"}),
	};
	const str out[] = {
		((str){0, (uint8_t*)""}),
		((str){1, (uint8_t*)"\xEC"}),
		((str){1, (uint8_t*)"G"}),
		((str){2, (uint8_t*)"B?"}),
		((str){2, (uint8_t*)"\x10\x20"}),
		((str){4, (uint8_t*)"\xFF\xFF\xFF\xFF"}),
		((str){12, (uint8_t*)"\xE7\xCF\x9B\xEB\xE8\x9B\x6F\xB1\x6F\xA9\xB6\xFF"}),
		((str){17, (uint8_t*)"\x3B\xFC\xD7\x65\x99\xBD\xAE\x6F\x3B\x8F\x5B\x71\x76\x6B\x56\xE4\xFF"}),
	};

	int huffman_encoder_error;

	const size_t n = 32;
	uint8_t buff[33]; /*n+1*/

	char match; int retval=0;
	size_t i,j;
	size_t length;

	printf("**\n** ENCODER\n**\n\n");

	for (i = 0; i < in_n; i++) {
		printf("Test %d:\n", (int)i);
		dump(in[i].ptr, in[i].length, '<', 1);

		/* aha! sending uninitialised buff to my function! */
		huffman_encoder_error = huffman_encode(in[i].ptr, in[i].length, NULL, buff, n, &length);

		if (huffman_encoder_error) {
			printf(" * error: %d\n", huffman_encoder_error);
		}

		if (length == out[i].length) {
			match = 1;
			for (j = 0; j < length; j++) {
				if (buff[j] != out[i].ptr[j]) {
					printf(" * mismatch at byte %d: got %02x, expected %02x\n", (int)j, buff[j], out[i].ptr[j]);
					match = 0;
					break;
				}
			}
		} else {
			match = 0;
			printf(" * returned %d, expected %d\n", (int)length, (int)out[i].length);
		}

		/*if (!match) {*/
			dump(buff, length, '>', 0);
			dump(out[i].ptr, out[i].length, '~', 0);
		/*}*/
		printf("\n");

		retval += (!(match)||!!huffman_encoder_error);
	}

	return retval;
}

int main() {
	return test_decode() + test_encode();
}

