
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../str.h"
#include "common.h"

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

const size_t __n = 1023;
uint8_t __buff[1024];
int test(const str *in, const str *out, int (*f)(uint8_t*,size_t,size_t*,uint8_t*,size_t,size_t*)) {
	int error;
	int fail;
	size_t j;
	size_t length;

	/* aha! sending uninitialised buff to my function! */
	error = (*f)(in->ptr, in->length, NULL, __buff, __n, &length);

	if (error) {
		printf(RED "error: %d" NORMAL "\n", error);
		return 1;
	}

	if (length == out->length) {
		fail = 0;
		for (j = 0; j < length; j++) {
			if (__buff[j] != out->ptr[j]) {
				printf(RED "mismatch at byte %d: got %02x, expected %02x" NORMAL "\n", (int)j, __buff[j], out->ptr[j]);
				fail = 1;
				break;
			}
		}
	} else {
		fail = 1;
		printf(RED "returned %d bytes, expected %d" NORMAL "\n", (int)length, (int)out->length);
	}

	if (fail) {
		dump_strp(in, '<', 1);
		dump(__buff, length, '>', 0);
		dump_strp(out, '~', 0);
		printf("\n");
	}
	if (!fail) {
		printf(GREEN "PASS" NORMAL "\n");
	}

	return fail;
}

int test_err(const str *in, int err, int (*f)(uint8_t*,size_t,size_t*,uint8_t*,size_t,size_t*)) {
	int error;
	size_t length;

	/* aha! sending uninitialised buff to my function! */
	error = (*f)(in->ptr, in->length, NULL, __buff, __n, &length);

	if (error != err) {
		printf(RED "error: %d (expected %d)" NORMAL "\n", error, err);
		dump_strp(in, '<', 1);
		dump(__buff, length, '>', 0);
		printf("\n");
		return 1;
	}

	printf(GREEN "PASS" NORMAL "\n");
	return 0;
}


