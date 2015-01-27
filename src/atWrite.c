#include "atOpen.h"
#include "atWrite.h"
#include "atDecEnc.h"
#include "atPrepare.h"
#include "atChecksum.h"
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>

#define START_SYMBOL 1
#define NEW_LINE     1
#define CHECKSUM     1

//#define DEBUG_WRITE

int atWrite(int fd, void* src, size_t size){
	int bytes, i;
	size_t msgSize = ((size + CHECKSUM) << 1) + START_SYMBOL + NEW_LINE;
	char hex[msgSize], _src[size + CHECKSUM];
	struct termios conf = {0};

	// correct message size, based on configuration
	if(AT_IS_BINARY) msgSize = size + (AT_IS_NON_CRC ? 0 : 1);

#ifdef DEBUG_WRITE
	printf("VVVVVVVVVVVVVVVVVVVVVVVVV\n");
	printf("atWrite() About to set port settings\n");
#endif

	// update serial settings if the message size
	// has changed since last sent
	// wait for the bang, and newline
	if(!AT_IS_BINARY){
		atPrepare(fd, 2);
#ifdef DEBUG_WRITE
		printf("atWrite() prepare 2\n");
#endif
	}

#ifdef DEBUG_WRITE
	printf("atWrite() set port settings\n\tabout to encode\n");
#endif

	// encode, and write the message
	memcpy(_src, src, size);
	
	if(AT_RXTX_SCRAM){
		for(i = size; i--;){
			if(rand() % 2048 < AT_RXTX_SCRAM){
				_src[i] = (unsigned char)0xFF & rand();
			}
		}
	}

	if(!AT_IS_NON_CRC) _src[size] = atChecksum(src, size);
	if(!AT_IS_BINARY) atEncode(_src, size + (AT_IS_NON_CRC ? 0 : CHECKSUM), hex);
	else memcpy(hex, _src, msgSize);

#ifdef DEBUG_WRITE
	printf("Writing >>%zu<<\n", msgSize);
	write(1, hex, msgSize);
#endif

	bytes = write(fd, hex, msgSize);
#ifdef DEBUG_WRITE
	printf("atWrite() wrote %d\n", bytes);
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^\n");
#endif
	return bytes;
};
