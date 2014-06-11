
#include <stdio.h>
#include <stdlib.h>

#include "../lib/hpack.h"

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
	const size_t good_n = 12;
	const str good_in[] = {
		((str){1, (uint8_t*)"\x00"}),
		((str){1, (uint8_t*)"\x01"}),
		((str){1, (uint8_t*)"\xFE"}),
		((str){2, (uint8_t*)"\xFF\x00"}),
		((str){2, (uint8_t*)"\xFF\x01"}),

		((str){1, (uint8_t*)"\x01"}),
		((str){1, (uint8_t*)"\x7E"}),
		((str){2, (uint8_t*)"\x7F\x00"}),
		((str){2, (uint8_t*)"\x7F\x7F"}),
		((str){3, (uint8_t*)"\x7F\x80\x01"}),
		((str){3, (uint8_t*)"\x7F\x81\x01"}),

		((str){6, (uint8_t*)"\xFF\xF2\x81\xC0\x80\x01"}),
	};
	const size_t good_pb[] = {
		8,8,8,8,8,
		7,7,7,7,7,7,
		4,
	};
	const uint8_t good_pf[] = {
		0,0,0,0,0,
		0,0,0,0,0,0,
		0xF0,
	};
	const uint64_t good_out[] = {
		0x00,
		0x01,
		0xFE,
		0xFF,
		0x100,

		0x01,
		0x7E,
		0x7F,
		0xFE,
		0xFF,
		0x100,

		0x10100101,
	};

	char match; int retval=0;
	size_t i;
	size_t result; uint8_t pf;

	printf("**\n** DECODER\n**\n\n");

	for (i = 0; i < good_n; i++) {
		printf("Test %d:\n", (int)i);
		dump(good_in[i].ptr, good_in[i].length, '<', 0);

		/* aha! sending uninitialised buff to my function! */
		result = hpack_decode_int(good_in[i].ptr, good_in[i].length, NULL, good_pb[i], NULL, &pf);

		if (hpack_decoder_error) {
			printf(" * error: %d\n", hpack_decoder_error);
		}

		if (result == good_out[i]) {
			if (pf == good_pf[i]) {
				match = 1;
			} else {
				match = 0;
				printf(" * prefix was %02x, expected %02x\n", pf, good_pf[i]);
			}
		} else {
			match = 0;
			printf(" * returned %x, expected %x\n", (int)result, (int)good_out[i]);
		}

		/*if (!match) {*/
			printf(" > %02X | %d\n", pf, (int)result);
			printf(" ~ %02X | %d\n", good_pf[i], (int)(good_out[i]));
		/*}*/
		printf("\n");

		retval += (!(match)||!!hpack_decoder_error);
	}
	return retval;
}

int test_encode() {
	const size_t good_n = 12;
	const uint64_t good_in[] = {
		0x00,
		0x01,
		0xFE,
		0xFF,
		0x100,

		0x01,
		0x7E,
		0x7F,
		0xFE,
		0xFF,
		0x100,

		0x10100101,
	};
	const size_t good_pb[] = {
		8,8,8,8,8,
		7,7,7,7,7,7,
		4,
	};
	const uint8_t good_pf[] = {
		0,0,0,0,0,
		0,0,0,0,0,0,
		0xF0,
	};
	const str good_out[] = {
		((str){1, (uint8_t*)"\x00"}),
		((str){1, (uint8_t*)"\x01"}),
		((str){1, (uint8_t*)"\xFE"}),
		((str){2, (uint8_t*)"\xFF\x00"}),
		((str){2, (uint8_t*)"\xFF\x01"}),

		((str){1, (uint8_t*)"\x01"}),
		((str){1, (uint8_t*)"\x7E"}),
		((str){2, (uint8_t*)"\x7F\x00"}),
		((str){2, (uint8_t*)"\x7F\x7F"}),
		((str){3, (uint8_t*)"\x7F\x80\x01"}),
		((str){3, (uint8_t*)"\x7F\x81\x01"}),

		((str){6, (uint8_t*)"\xFF\xF2\x81\xC0\x80\x01"}),
	};

	const size_t bad_n = 4;
	const uint64_t bad_in[] = {
		0,
		0,

		0,
		0,
	};
	const size_t bad_pb[] = {
		0, /* good_pb < 1 */
		9, /* good_pb > 8 */

		8,
		7,
	};
	const uint8_t bad_pf[] = {
		0,
		0,

		0x01, /* prefix sets masked bits */
		0xC0, /* prefix sets masked bits */
	};

	const size_t n = 32;
	uint8_t buff[33]; /*n+1*/

	char match; int retval=0;
	size_t i,j;
	size_t result;

	printf("**\n** ENCODER\n**\n\n");

	for (i = 0; i < good_n; i++) {
		printf("Test %d:\n", (int)i);
		printf(" < %08x [%3d]; %d | %02x\n", (unsigned int)(good_in[i]), (int)(good_in[i]), (int)(good_pb[i]), good_pf[i]);

		/* aha! sending uninitialised buff to my function! */
		result = hpack_encode_int(good_in[i], good_pb[i], good_pf[i], buff, n, NULL);

		if (hpack_encoder_error) {
			printf(" * error: %d\n", hpack_encoder_error);
		}

		if (result == good_out[i].length) {
			match = 1;
			for (j = 0; j < result; j++) {
				if (buff[j] != good_out[i].ptr[j]) {
					printf(" * mismatch at byte %d: got %02x, expected %02x\n", (int)j, buff[j], good_out[i].ptr[j]);
					match = 0;
					break;
				}
			}
		} else {
			match = 0;
			printf(" * returned %d, expected %d\n", (int)result, (int)good_out[i].length);
		}

		/*if (!match) {*/
			dump(buff, result, '>', 0);
			dump(good_out[i].ptr, good_out[i].length, '~', 0);
		/*}*/
		printf("\n");

		retval += (!(match)||!!hpack_encoder_error);
	}

	for (i = 0; i < bad_n; i++) {
		printf("Test %d:\n", (int)(good_n+i));
		printf(" < %08x [%3d]; %d | %02x\n", (unsigned int)(bad_in[i]), (int)(bad_in[i]), (int)(bad_pb[i]), bad_pf[i]);

		/* aha! sending uninitialised buff to my function! */
		result = hpack_encode_int(bad_in[i], bad_pb[i], bad_pf[i], buff, n, NULL);

		if (hpack_encoder_error) {
			printf(" * error: %d\n", hpack_encoder_error);
		}

		/*dump(buff, result, '>', 1);*/
		printf("\n");

		retval += (!hpack_encoder_error);
	}

	return retval;
}

int main() {
	return test_decode() + test_encode();
}

