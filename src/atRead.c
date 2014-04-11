#include "atRead.h"
#include "atDecEnc.h"
#include "atPrepare.h"
#include <stdio.h>
#include <termios.h>
#include <strings.h>
#include <unistd.h>

int _checkMessage(char* dst, int bytes){
	if(dst[0] != '$') return -1;
	if(bytes != _AT_LAST_SIZE) return -2;

	return bytes - 1;
};

// TODO allow time out
int atRead(int fd, void* dst, size_t size){
	int bytes = 0;
	char hex[1024];

	// update serial settings if the message size
	// has changed since last sent
	atPrepare(fd, 1 + (size << 1));

	// perform the read, continue reading until a message is legit
	//do{
		bytes = read(fd, hex, _AT_LAST_SIZE);
		printf("--%s\n", hex);
		atDecode(dst, size, hex);

		printf("atRead() %d bytes decoded %s\n", bytes, hex);
	//}while(!bytes);

	// send the ack
	if(bytes) write(fd, "!\n", 2);

	return bytes;
};
