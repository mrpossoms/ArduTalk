#ifndef AT_READ
#define AT_READ
#include <stdlib.h>

extern size_t _AT_MSG_SIZE;

int atRead(int fd, void* dst, size_t size);

#endif
