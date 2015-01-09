#include "atPrepare.h"
#include "atOpen.h"
#include <math.h>

#define START_SYMBOL 1
#define NEW_LINE     1
#define CHECKSUM     1

speed_t _AT_BAUD;
size_t _AT_LAST_SIZE;

int atPrepare(int fd, size_t size){
	struct termios conf = {0};

	// account for library configuration
	if(_AT_LIB_CONF & AT_BIN){
		// Binary message transmission
		// check to see if checksum has been disabled
		size += _AT_LIB_CONF & AT_NCHKSUM ? 0 : 1;
	}
	else{
		// ascii message transmission
		size = ((size + CHECKSUM) << 1) + START_SYMBOL;
	}

	if(_AT_LAST_SIZE == size) return 0;
	
	tcgetattr(fd, &conf);
	// wait for two times the number of bytes (ascii hex)
	// and a start and stop character
	// set the timer to wait for 8 bits plus a stop bit for each byte
	// transmitted. Multiply this by ten for the 0.1 second granularity
	// divide by baud rate

	// specify timeouts
	if(_AT_LIB_CONF & AT_BLOCKING){
		//conf.c_cc[VTIME] = 20; // two second time-outs
	}
	else{
		conf.c_cc[VTIME] = (((size << 3) + 1) * 10) / _AT_BAUD;
		conf.c_cc[VTIME] += !conf.c_cc[VTIME] ? 1 : 0;
	}

	// set expected bytes and apply parameters
	conf.c_cc[VMIN] = _AT_LAST_SIZE = size;
	tcsetattr(fd, TCSANOW, &conf);

	return 0;
}
