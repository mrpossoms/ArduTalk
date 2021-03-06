#include "atOpen.h"
#include "atRead.h"
#include "atDecEnc.h"
#include "atPrepare.h"
#include "atChecksum.h"
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define START_SYMBOL 1
#define NEW_LINE     1
#define CHECKSUM     1

//#define DEBUG_READ
#define SEC_2_MICRO_SEC 1000000.0

size_t atAvailable(int fd){
	size_t bytes = 0;
	ioctl(fd, FIONREAD, &bytes);
	return bytes;
}

// TODO allow time out
int atRead(int fd, void* dst, size_t size){
	int bytes = 0, i = 0;
	size_t msgSize = ((size + CHECKSUM) << 1) + START_SYMBOL;
	char msg[msgSize], _read[size + CHECKSUM];

	// correct message size, based on configuration
	if(AT_IS_BINARY) msgSize = size + (AT_IS_NON_CRC ? 0 : 1);

	// update serial settings if the message size
	// has changed since last sent
	atPrepare(fd, size);

	// inital time out
	{
		struct timeval timeout = { 1, 0 }; // 1 second timeout
		fd_set readFd;
		FD_ZERO(&readFd);
		FD_SET(fd, &readFd);

		if(!select(fd + 1, &readFd, NULL, NULL, &timeout)){
			return -2; // timeout
		}
	}

	// read message. wait for a start token, or correct
	// number of bytes
	for(i = size; i--;){
		bytes = read(fd, msg, _AT_LAST_SIZE);
		if(AT_IS_BINARY || msg[0] == '$') break;
		else if (bytes == size) break;
	}

	// we must assure we are getting the number of bytes expected
	//if(AT_IS_BINARY) assert(bytes >= size);
	
#ifdef DEBUG_READ
	printf("\natRead() got %zu bytes\n", _AT_LAST_SIZE);
	{
		int i = 0;

		for(i = 0; i < _AT_LAST_SIZE; i++)
			printf("%02x ", msg[i]);
		printf("\n");

		printf("[[");
		for(i = 0; i < _AT_LAST_SIZE; i++)
			printf("%c ", msg[i]);

		printf("]]\n");
	}
#endif
	// Only decode if not using binary messaging
	if(!AT_IS_BINARY)
	atDecode(_read, size + (AT_IS_NON_CRC ? 0 : CHECKSUM), msg);

	// compute and check checksum. If used.
	if(!AT_IS_NON_CRC && _read[size] != atChecksum(_read, size)){
		// this message is corrupted
#ifdef DEBUG_READ
		printf("atRead() - message corrupted\n");
#endif
		return -1;
	}

	// The checksums match, we are good
	memcpy(dst, msg, size);

#ifdef DEBUG_READ
	printf("atRead() %d bytes decoded %s\n", bytes, msg);
#endif
	usleep(1000);
	return bytes;
};
