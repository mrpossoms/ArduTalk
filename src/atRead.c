#include "atOpen.h"
#include "atRead.h"
#include "atDecEnc.h"
#include "atPrepare.h"
#include "atChecksum.h"
#include <stdio.h>
#include <termios.h>
#include <strings.h>
#include <unistd.h>

#define START_SYMBOL 1
#define NEW_LINE     1
#define CHECKSUM     1

//#define DEBUG_READ

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

	// read message. wait for a start token, or correct
	// number of bytes
	for(i = size; i--;){
		bytes = read(fd, msg, _AT_LAST_SIZE);
		if(AT_IS_BINARY || msg[0] == '$') break;
		else if (bytes == size) break;
	}
	
#ifdef DEBUG_READ
	printf("atRead() got %d bytes\n", _AT_LAST_SIZE);
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
	return bytes;
};
