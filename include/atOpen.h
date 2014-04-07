#ifndef AT_OPEN
#define AT_OPEN
#include <termios.h>

#define AT_ERR_UNABLE_TO_OPEN 0x00


extern struct termios _AT_OLD_CONFIG;

int atOpen(const char* dev, speed_t baud);
#endif
