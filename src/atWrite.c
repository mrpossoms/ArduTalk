#include "atWrite.h"
#include "atDecEnc.h"
#include <stdio.h>
#include <termios.h>
#include <strings.h>
#include <unistd.h>

int atWrite(int fd, void* src, size_t size){
	int bytes = 0;
	char hex[1024]; // TODO something more intelligent perhaps?
	struct termios conf = {0};

	// update serial settings if the message size
	// has changed since last sent
	// wait for the bang, and newline
	atPrepare(fd, 2);

	// encode, and write the message
	atEncode(src, size, hex);
	bytes = write(fd, hex, size);
	bzero(hex, 1024);

	// perform the ACK read
	read(fd, hex, 2);
	
	return bytes;
};
