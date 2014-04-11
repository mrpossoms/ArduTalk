#include "atWrite.h"
#include "atDecEnc.h"
#include "atPrepare.h"
#include <stdio.h>
#include <termios.h>
#include <strings.h>
#include <unistd.h>

int atWrite(int fd, void* src, size_t size){
	int bytes = 0;
	char hex[1024], ack[2]; // TODO something more intelligent perhaps?
	struct termios conf = {0};
	size_t msgSize = (size << 1) + 2;

	printf("atWrite() About to set port settings\n");

	// update serial settings if the message size
	// has changed since last sent
	// wait for the bang, and newline
	atPrepare(fd, 2);


	printf("atWrite() set port settings\n\tabout to encode\n");

	// encode, and write the message
	atEncode(src, size, hex);

	printf("Writing %s %zu\n", hex, msgSize);

	// resend message until ack'd
	//do{
		write(fd, hex, msgSize);
		printf("atWrite() written\n");
		bzero(hex, 1024);

		// perform the ACK read
		bytes = read(fd, ack, 2);

		printf("atWrite() read %d bytes ", bytes);
		printf("%02x %02x\n",ack[0], ack[1] );
		printf("%c %c\n",ack[0], ack[1] );
	//}while(ack[0] != '!');

	printf("atWrite() Done\n");
	return bytes;
};
