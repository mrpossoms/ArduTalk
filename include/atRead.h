#ifndef AT_READ
#define AT_READ
#include <stdlib.h>
#include <sys/ioctl.h>

size_t atAvailable(int fd);
int atRead(int fd, void* dst, size_t size);

#endif
