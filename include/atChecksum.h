#ifndef AT_CHECKSUM
#define AT_CHECKSUM
#include <unistd.h>

unsigned char atChecksum(void* data, size_t size);
#endif
