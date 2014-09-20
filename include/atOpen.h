#ifndef AT_OPEN
#define AT_OPEN
#include <termios.h>

#define AT_ERR_UNABLE_TO_OPEN 0x00

#define AT_BIN      0x01
#define AT_NCHKSUM  0x02
#define AT_BLOCKING 0x03

#define AT_IS_BINARY  _AT_LIB_CONF & AT_BIN
#define AT_IS_NON_CRC _AT_LIB_CONF & AT_NCHKSUM
#define AT_IS_NORMAL  _AT_LIB_CONF == 0

extern struct termios _AT_OLD_CONFIG;
extern int _AT_LIB_CONF;

int  atOpen(const char* dev, speed_t baud, int flags);
void atConfig(int fd, int flags);

#endif
