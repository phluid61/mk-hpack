
#include <stdint.h>
#ifndef HAVE_STR

typedef struct str {
	size_t length;
	uint8_t *ptr;
} str;

#define STR_C(l,p) ((str){(size_t)(l),(uint8_t*)(p)})

#define HAVE_STR 1
#endif

