#include "atOpen.h"
#include "atPrepare.h"
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>

struct termios _AT_OLD_CONFIG;
int _AT_LIB_CONF;

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
		printf("%x\n", conf.c_iflag);
		conf.c_iflag |= IGNBRK;
		conf.c_cflag |= CLOCAL;
		// no hardware flow control
		conf.c_cflag &= ~CRTSCTS;
		// no software flow control  
		conf.c_iflag &= ~(IXON | IXOFF | IXANY);
		printf("%x\n", conf.c_iflag);
		tcsetattr(fd, TCSANOW, &conf);
	}
}
//---------------------------------------------------------------
int atOpen(const char* dev, speed_t baud){
	int fd = -1;
	struct termios config = {0};

	printf("atOpen() entered opening %s\n", dev);

	// set default behavior for library
	_AT_LIB_CONF = 0;

	// try to open the port, set errno if it fails
	if((fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
		errno = AT_ERR_UNABLE_TO_OPEN;
		return fd;		
	}

	assert(isatty(fd));

	printf("fd: %d\n", fd);

	// save the old config settings for this port
	tcgetattr(fd, &_AT_OLD_CONFIG);
	
	_setPortConf(&config, fd, baud);	

	return fd;
}
