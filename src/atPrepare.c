#include "atPrepare.h"
#include <math.h>

speed_t _AT_BAUD;
size_t _AT_LAST_SIZE;

int atPrepare(int fd, size_t size){
	struct termios conf = {0};

	size = (size << 1) + 1;
	if(_AT_LAST_SIZE == size) return 0;
	
	tcgetattr(fd, &conf);
	// wait for two times the number of bytes (ascii hex)
	// and a start and stop character
	// set the timer to wait for 8 bits plus a stop bit for each byte
	// transmitted. Multiply this by ten for the 0.1 second granularity
	// divide by baud rate
	conf.c_cc[VTIME] = (((size << 3) + 1) * 10) / _AT_BAUD;
	conf.c_cc[VTIME] += !conf.c_cc[VTIME] ? 1 : 0;
	conf.c_cc[VMIN] = _AT_LAST_SIZE;
	tcsetattr(fd, TCSANOW, &conf);

	return 0;
}
