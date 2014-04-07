#include "atOpen.h"
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

struct termios _AT_OLD_CONFIG;

void _setPortConf(struct termios *conf, int fd, speed_t baud){
	// set speeds
	cfsetospeed(conf, baud);
	cfsetispeed(conf, baud);

	// set control flags
	conf->c_cflag |= CS8 | CLOCAL | CREAD;
	
	// set IO flags
	conf->c_iflag = IGNPAR;
	conf->c_oflag = 0;

	// set to cannonical mode don't use timer
	conf->c_lflag = ICANON;
	conf->c_cc[VTIME] = 0;

	// flush port, apply settings
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, conf);
}
//---------------------------------------------------------------
int atOpen(const char* dev, speed_t baud){
	int fd = -1;
	struct termios config = {0};

	// try to open the port, set errno if it fails
	if((fd = open(dev, O_RDWR | O_NOCTTY)) < 0){
		errno = AT_ERR_UNABLE_TO_OPEN;
		return fd;		
	}

	// save the old config settings for this port
	tcgetattr(fd, &_AT_OLD_CONFIG);
	
	_setPortConf(&config, fd, baud);	

	return fd;
}
