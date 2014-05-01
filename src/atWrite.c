#include "atOpen.h"
#include "atWrite.h"
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

int atWrite(int fd, void* src, size_t size){
	int bytes = 0;
	size_t msgSize = ((size + CHECKSUM) << 1) + START_SYMBOL + NEW_LINE;
	char hex[msgSize], _src[size + CHECKSUM];
	struct termios conf = {0};

	// correct message size, based on configuration
	if(AT_IS_BINARY) msgSize = size + (AT_IS_NON_CRC ? 0 : 1);

	printf("atWrite() About to set port settings\n");

	// update serial settings if the message size
	// has changed since last sent
	// wait for the bang, and newline
	atPrepare(fd, 2);

	printf("atWrite() set port settings\n\tabout to encode\n");

	// encode, and write the message
	memcpy(_src, src, size);
	
	if(!AT_IS_NON_CRC) _src[size] = atChecksum(src, size);
	if(!AT_IS_BINARY) atEncode(_src, size + CHECKSUM, hex);

	printf("Writing %s %zu\n", hex, msgSize);

	write(fd, hex, msgSize);
	printf("atWrite() written\n");
	return bytes;
};
