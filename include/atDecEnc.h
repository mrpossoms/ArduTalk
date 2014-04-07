#ifndef AT_DEC_ENC
#define AT_DEC_ENC
#include <stdlib.h>

int atEncode(void* src, size_t size, char* hex);
int atDecode(void* dst, size_t size, char* hex);

#endif
