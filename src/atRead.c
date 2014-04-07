#include "atRead.h"
#include "atDecEnc.h"
#include <stdio.h>
#include <termios.h>
//#include <fcntl.h>
#include <strings.h>
#include <unistd.h>

size_t _AT_MSG_SIZE;

int _checkMessage(char* dst, int bytes){
	if(dst[0] != '$') return -1;
	if(bytes != _AT_MSG_SIZE) return -2;

	return bytes - 1;
};

// TODO allow time out
int atRead(int fd, void* dst, size_t size){
	int bytes = 0;
	char hex[1024];

	// update serial settings if the message size
	// has changed since last sent
	/*if(size != _AT_MSG_SIZE)*/{
		struct termios conf = {0};

		tcgetattr(fd, &conf);
		// wait for two times the number of bytes (ascii hex)
		// and a start and stop character
		conf.c_cc[VMIN] = _AT_MSG_SIZE = (size << 1) + 1;
		tcsetattr(fd, TCSANOW, &conf);
	}

	// perform the read
	bytes = read(fd, hex, _AT_MSG_SIZE);
	atDecode(dst, size, hex);

	if((bytes = _checkMessage(hex, bytes)) < 0)
		bzero(dst, size);	

	// send the ack
	write(fd, "!\n", 2);

	return bytes;
};
