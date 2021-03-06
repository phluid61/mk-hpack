
#include <stdio.h>
#include <stdlib.h>

#include "../lib/hpack.h"
#include "../str.h"

#include "common.h"

int test_decode_int() {
	const size_t good_n = 12;
	const str good_in[] = {
		STR_C(1, "\x00"),
		STR_C(1, "\x01"),
		STR_C(1, "\xFE"),
		STR_C(2, "\xFF\x00"),
		STR_C(2, "\xFF\x01"),

		STR_C(1, "\x01"),
		STR_C(1, "\x7E"),
		STR_C(2, "\x7F\x00"),
		STR_C(2, "\x7F\x7F"),
		STR_C(3, "\x7F\x80\x01"),
		STR_C(3, "\x7F\x81\x01"),

		STR_C(6, "\xFF\xF2\x81\xC0\x80\x01"),
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
	const HPACK_INT_T good_out[] = {
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

	int hpack_decoder_error;

	char match; int retval=0;
	size_t i;
	HPACK_INT_T result; uint8_t pf;

	printf("\n" BOLD "**\n** DECODE INT\n**\n" NORMAL "\n");

	for (i = 0; i < good_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)i);

		/* aha! sending uninitialised buff to my function! */
		hpack_decoder_error = hpack_decode_int(good_in[i].ptr, good_in[i].length, NULL, good_pb[i], &result, &pf);

		if (hpack_decoder_error) {
			printf(RED "error: %d" NORMAL "\n", hpack_decoder_error);
			retval ++;
		} else {

			if (result == good_out[i]) {
				if (pf == good_pf[i]) {
					match = 1;
				} else {
					match = 0;
					printf(RED "prefix was %02x, expected %02x" NORMAL "\n", pf, good_pf[i]);
				}
			} else {
				match = 0;
				printf(RED "returned %x bytes, expected %x" NORMAL "\n", (int)result, (int)good_out[i]);
			}

			if (!match) {
				dump_str(good_in[i], '<', 0);
				printf(" > %02X | %d\n", pf, (int)result);
				printf(" ~ %02X | %d\n", good_pf[i], (int)(good_out[i]));
				printf("\n");
			} else {
				printf(GREEN "PASS" NORMAL "\n");
			}

			retval += (!match);
		}
	}
	return retval;
}

int test_encode_int() {
	const size_t good_n = 12;
	const HPACK_INT_T good_in[] = {
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
		STR_C(1, "\x00"),
		STR_C(1, "\x01"),
		STR_C(1, "\xFE"),
		STR_C(2, "\xFF\x00"),
		STR_C(2, "\xFF\x01"),

		STR_C(1, "\x01"),
		STR_C(1, "\x7E"),
		STR_C(2, "\x7F\x00"),
		STR_C(2, "\x7F\x7F"),
		STR_C(3, "\x7F\x80\x01"),
		STR_C(3, "\x7F\x81\x01"),

		STR_C(6, "\xFF\xF2\x81\xC0\x80\x01"),
	};

	const size_t bad_n = 4;
	const HPACK_INT_T bad_in[] = {
		0,
		0,

		0,
		0,
	};
	const size_t bad_pb[] = {
		0, /* < 1 */
		9, /* > 8 */

		8,
		7,
	};
	const uint8_t bad_pf[] = {
		0,
		0,

		0x01, /* prefix sets masked bits */
		0xC0, /* prefix sets masked bits */
	};
	const int bad_out[] = {
		4, /* invalid prefix */
		4, /* invalid prefix */

		4, /* invalid prefix */
		4, /* invalid prefix */
	};

	int hpack_encoder_error;

	const size_t n = 32;
	uint8_t buff[33]; /*n+1*/

	char match; int retval=0;
	size_t i,j;
	size_t length;

	printf("\n" BOLD "**\n** ENCODE INT\n**\n" NORMAL "\n");

	for (i = 0; i < good_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)i);

		/* aha! sending uninitialised buff to my function! */
		hpack_encoder_error = hpack_encode_int(good_in[i], good_pb[i], good_pf[i], buff, n, &length);

		if (hpack_encoder_error) {
			printf(RED "error: %d" NORMAL "\n", hpack_encoder_error);
			retval ++;
		} else {

			if (length == good_out[i].length) {
				match = 1;
				for (j = 0; j < length; j++) {
					if (buff[j] != good_out[i].ptr[j]) {
						printf(RED "mismatch at byte %d: got %02x, expected %02x" NORMAL "\n", (int)j, buff[j], good_out[i].ptr[j]);
						match = 0;
						break;
					}
				}
			} else {
				match = 0;
				printf(RED "returned %d, expected %d" NORMAL "\n", (int)length, (int)good_out[i].length);
			}

			if (!match) {
				printf(" < %08x [%3d]; %d | %02x\n", (unsigned int)(good_in[i]), (int)(good_in[i]), (int)(good_pb[i]), good_pf[i]);
				dump(buff, length, '>', 0);
				dump_str(good_out[i], '~', 0);
				printf("\n");
			} else {
				printf(GREEN "PASS" NORMAL "\n");
			}

			retval += (!match);
		}
	}

	for (i = 0; i < bad_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)(good_n+i));

		/* aha! sending uninitialised buff to my function! */
		hpack_encoder_error = hpack_encode_int(bad_in[i], bad_pb[i], bad_pf[i], buff, n, &length);

		if (hpack_encoder_error != bad_out[i]) {
			printf(RED "error: %d (expected %d)" NORMAL "\n", hpack_encoder_error, bad_out[i]);
			printf(" < %08x [%3d]; %d | %02x\n", (unsigned int)(bad_in[i]), (int)(bad_in[i]), (int)(bad_pb[i]), bad_pf[i]);
			/*dump(buff, length, '>', 1);*/
			printf("\n");
			retval ++;
		} else {
			printf(GREEN "PASS" NORMAL "\n");
		}
	}

	return retval;
}


int test_hpack_encode_raw_str() {
	const size_t in_n = 4;
	const str in[] = {
		STR_C(0, ""),
		STR_C(1, "."),
		STR_C(5, "\x00\x20\x00\xFF\x30"),
		STR_C(512, \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
			 ),
	};
	const str out[] = {
		STR_C(1, "\x00"),
		STR_C(2, "\x01."),
		STR_C(6, "\x05\x00\x20\x00\xFF\x30"),
		STR_C(515, "\x7F\x81\x03" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
			 ),
	};

	int retval=0;
	size_t i;

	printf("\n" BOLD "**\n** ENCODE RAW STR\n**\n" NORMAL "\n");

	for (i = 0; i < in_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)i);
		retval += test(&(in[i]), &(out[i]), hpack_encode_raw_str);
	}

	if (retval == 0) {
		printf(GREEN);
	} else if (retval < in_n) {
		printf(YELLOW);
	} else {
		printf(RED);
	}
	printf("Passed %d of %d tests." NORMAL "\n\n", (int)(in_n-retval), (int)in_n);

	return retval;
}

int test_hpack_encode_huff_str() {
	const size_t in_n = 4;
	const str in[] = {
		STR_C(0, ""),
		STR_C(1, "\xF9"),
		STR_C(5, "\x00\x20\x00\xFF\x30"),
		STR_C(512, \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
			 ),
	};
	const str out[] = {
		STR_C(1, "\x80"),
		STR_C(2, "\x81\xF9"),
		STR_C(6, "\x85\x00\x20\x00\xFF\x30"),
		STR_C(515, "\xFF\x81\x03" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
				"................................................................" \
			 ),
	};

	int retval=0;
	size_t i;

	printf("\n" BOLD "**\n** ENCODE HUFF STR\n**\n" NORMAL "\n");

	for (i = 0; i < in_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)i);
		retval += test(&(in[i]), &(out[i]), hpack_encode_huff_str);
	}

	if (retval == 0) {
		printf(GREEN);
	} else if (retval < in_n) {
		printf(YELLOW);
	} else {
		printf(RED);
	}
	printf("Passed %d of %d tests." NORMAL "\n\n", (int)(in_n-retval), (int)in_n);

	return retval;
}

int main() {
	return test_decode_int() + test_encode_int()
		+ test_hpack_encode_raw_str() + test_hpack_encode_huff_str();
}

