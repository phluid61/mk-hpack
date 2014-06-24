
#include <stdint.h>
#ifndef HAVE_TEST_COMMON

#define BOLD "\x1B[1m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[93m"
#define NORMAL "\x1B[0m"

void dump(uint8_t* buff, size_t n, char token, char literal);
#define dump_str(str,token,literal) dump((str).ptr,(str).length,(token),(literal))
#define dump_strp(str,token,literal) dump((str)->ptr,(str)->length,(token),(literal))

int test(const str *in, const str *out, int (*f)(uint8_t*,size_t,size_t*,uint8_t*,size_t,size_t*));
int test_err(const str *in, int err, int (*f)(uint8_t*,size_t,size_t*,uint8_t*,size_t,size_t*));

#define HAVE_TEST_COMMON 1
#endif

