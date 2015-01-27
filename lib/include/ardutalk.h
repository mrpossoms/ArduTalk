#ifndef AT_READ
#define AT_READ
#include <stdlib.h>
#include <sys/ioctl.h>

size_t atAvailable(int fd);
int atRead(int fd, void* dst, size_t size);

#endif
#ifndef AT_PREPARE
#define AT_PREPARE
#include <termios.h>
#include <unistd.h>

extern speed_t _AT_BAUD;
extern size_t _AT_LAST_SIZE;

int atPrepare(int fd, size_t size);
#endif
#ifndef AT_DEC_ENC
#define AT_DEC_ENC
#include <stdlib.h>

int atEncode(void* src, size_t size, char* hex);
int atDecode(void* dst, size_t size, char* hex);

#endif
#ifndef AT_OPEN
#define AT_OPEN
#include <termios.h>

#define AT_ERR_UNABLE_TO_OPEN 0x00

#define AT_BIN      0x01
#define AT_NCHKSUM  0x02
#define AT_BLOCKING 0x04

#define AT_IS_BINARY  _AT_LIB_CONF & AT_BIN
#define AT_IS_NON_CRC _AT_LIB_CONF & AT_NCHKSUM
#define AT_IS_NORMAL  _AT_LIB_CONF == 0

extern struct termios _AT_OLD_CONFIG;
extern int _AT_LIB_CONF;

extern unsigned char AT_RXTX_SCRAM;

int  atOpen(const char* dev, speed_t baud, int flags);
void atConfig(int fd, int flags);

#endif
#ifndef AT_CHECKSUM
#define AT_CHECKSUM
#include <unistd.h>

unsigned char atChecksum(void* data, size_t size);
#endif
#ifndef AT_WRITE
#define AT_WRITE
#include <stdlib.h>

int atWrite(int fd, void* dst, size_t size);

#endif
