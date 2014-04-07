#ifndef AT_PREPARE
#define AT_PREPARE
#include <termios.h>
#include <unistd.h>

extern speed_t _AT_BAUD;
extern size_t _AT_LAST_SIZE;

int atPrepare(int fd, size_t size);
#endif
