#include "atOpen.h"
#include "atPrepare.h"
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>

#define DEBUG_OPEN

struct termios _AT_OLD_CONFIG;
int _AT_LIB_CONF;

unsigned char AT_RXTX_SCRAM;

void _setPortConf(struct termios *conf, int fd, speed_t baud){
	// set speeds
	_AT_BAUD = baud;
	cfsetospeed(conf, baud);
	cfsetispeed(conf, baud);

	// set control flags
	conf->c_cflag |= CS8 | CLOCAL | CREAD;
	
	// set IO flags
	conf->c_iflag = IGNPAR;
	conf->c_oflag = 0;

	// set to cannonical mode don't use timer
	conf->c_lflag = ICANON;

	// flush port, apply settings
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, conf);
}
//---------------------------------------------------------------
void atConfig(int fd, int flags){
	_AT_LIB_CONF = flags;

	if(AT_IS_BINARY){
		struct termios conf = {0};

		tcgetattr(fd, &conf);
		cfmakeraw(&conf);

#ifdef DEBUG_OPEN
		printf("%lx\n", (unsigned long)conf.c_iflag);
#endif

		conf.c_iflag |= IGNBRK;
		conf.c_cflag |= CLOCAL;
		// no hardware flow control
		conf.c_cflag &= ~CRTSCTS;
		// no software flow control  
		conf.c_iflag &= ~(IXON | IXOFF | IXANY);

#ifdef DEBUG_OPEN
		printf("%lx\n", (unsigned long)conf.c_iflag);
#endif
		
		tcsetattr(fd, TCSANOW, &conf);
	}
}
//---------------------------------------------------------------
int atOpen(const char* dev, speed_t baud, int flags){
	int fd = -1;
	int isBlocking = (flags & AT_BLOCKING) ? 0 : O_NONBLOCK;
	struct termios config = {0};

	AT_RXTX_SCRAM = 0;

	printf("atOpen() entered opening %s with flags %x\n", dev, flags);

	if(flags & AT_BLOCKING){
		printf("Configuring to block...\n");
	}

	// set default behavior for library
	_AT_LIB_CONF = 0;

	// try to open the port, set errno if it fails
	if((fd = open(dev, O_RDWR | O_NOCTTY | isBlocking)) < 0){
		errno = AT_ERR_UNABLE_TO_OPEN;
		return fd;		
	}

	assert(isatty(fd));

	printf("fd: %d\n", fd);

	// save the old config settings for this port
	tcgetattr(fd, &_AT_OLD_CONFIG);
	_setPortConf(&config, fd, baud);

	if(flags){	
		atConfig(fd, flags);
	}

	return fd;
}
