
#include <stdio.h>
#include <stdlib.h>

#include "../lib/hpack.h"
#include "../str.h"

#include "common.h"

int test_decode() {
	const size_t good_n = 7;
	const str good_in[] = {
		STR_C(0, ""),
		STR_C(1, "\xF9"),
		STR_C(1, "g"),
		STR_C(2, "!?"),
		STR_C(4, "\xFF\xFF\xFF\xCF"),
		STR_C(12, "\xF1\xE3\xC2\xE5\xF2\x3A\x6B\xA0\xAB\x90\xF4\xFF"),
		STR_C(16, "\x61\x7f\x05\xa2\x85\xba\xd4\x7f\x15\x31\x39\xd0\x95\xa9\x2a\x47"),
	};
	const str good_out[] = {
		STR_C(0, ""),
		STR_C(1, "*"),
		STR_C(1, "3"),
		STR_C(2, "cc"),
		STR_C(1, "\x7F"),
		STR_C(15, "www.example.com"),
		STR_C(22, "/.well-known/host-meta"),
	};

	const size_t bad_n = 2;
	const str bad_in[] = {
		STR_C(1, "\x66"), /* \x67 would be '3', but wrong padding */
		STR_C(4, "\xFF\xFF\xFF\xFF"), /* valid encoding of EOS */
	};
	const int bad_out[] = {
		2, /* truncated */
		3, /* EOS */
	};

	int retval=0;
	size_t i;

	printf("\n" BOLD "**\n** HUFFMAN DECODE\n**\n" NORMAL "\n");

	for (i = 0; i < good_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)i);
		retval += test(&(good_in[i]), &(good_out[i]), huffman_decode);
	}

	for (i = 0; i < bad_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)(good_n+i));
		retval += test_err(&(bad_in[i]), bad_out[i], huffman_decode);
	}

	if (retval == 0) {
		printf(GREEN);
	} else if (retval < (good_n+bad_n)) {
		printf(YELLOW);
	} else {
		printf(RED);
	}
	printf("Passed %d of %d tests." NORMAL "\n\n", (int)(good_n+bad_n-retval), (int)(good_n+bad_n));

	return retval;
}

int test_encode() {
	const size_t in_n = 7;
	const str in[] = {
		STR_C(0, ""),
		STR_C(1, "*"),
		STR_C(1, "3"),
		STR_C(2, "cc"),
		STR_C(1, "\x7F"),
		STR_C(15, "www.example.com"),
		STR_C(22, "/.well-known/host-meta"),
	};
	const str out[] = {
		STR_C(0, ""),
		STR_C(1, "\xF9"),
		STR_C(1, "g"),
		STR_C(2, "!?"),
		STR_C(4, "\xFF\xFF\xFF\xCF"),
		STR_C(12, "\xF1\xE3\xC2\xE5\xF2\x3A\x6B\xA0\xAB\x90\xF4\xFF"),
		STR_C(16, "\x61\x7f\x05\xa2\x85\xba\xd4\x7f\x15\x31\x39\xd0\x95\xa9\x2a\x47"),
	};

	int retval=0;
	size_t i;

	printf("\n" BOLD "**\n** HUFFMAN ENCODE\n**\n" NORMAL "\n");

	for (i = 0; i < in_n; i++) {
		printf(BOLD "Test %d: " NORMAL, (int)i);
		retval += test(&(in[i]), &(out[i]), huffman_encode);
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
	return test_decode() + test_encode();
}

