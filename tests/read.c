#include "ardutalk.h"
#include <stdio.h>
#include <strings.h>

int main(int argc, char* argv[]){
	int fd = atOpen(argv[1], 115200, AT_BIN | AT_BLOCKING);
	unsigned char b;
	printf("fd %d\n", fd);

	atRead(fd, &b, sizeof(unsigned char));

	printf("%c\n", b);

	return 0;
}
