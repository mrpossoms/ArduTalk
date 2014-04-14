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

// TODO allow time out
int atRead(int fd, void* dst, size_t size){
	int bytes = 0, i = 0;
	size_t msgSize = ((size + CHECKSUM) << 1) + START_SYMBOL;
	char hex[msgSize], _read[size + CHECKSUM];

	// update serial settings if the message size
	// has changed since last sent
	atPrepare(fd, size);

	for(i = size; i--;){
		bytes = read(fd, hex, _AT_LAST_SIZE);
		if(hex[0] == '$') break;
	}
	
	printf("atRead() got %d bytes\n", _AT_LAST_SIZE);

	{
		int i = 0;

		for(i = 0; i < _AT_LAST_SIZE; i++)
			printf("%02x ", hex[i]);
		printf("\n");

		printf("[[");
		for(i = 0; i < _AT_LAST_SIZE; i++)
			printf("%c ", hex[i]);

		printf("]]\n");
	}

	atDecode(_read, size + CHECKSUM, hex);

	if(_read[size] != atChecksum(_read, size)){
		// this message is corrupted
		printf("atRead() - message corrupted\n");
		return -1;
	}

	// The checksums match, we are good
	memcpy(dst, _read, size);

	printf("atRead() %d bytes decoded %s\n", bytes, hex);

	return bytes;
};
