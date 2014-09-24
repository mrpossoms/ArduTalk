#include "ardutalk.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	int fd = atOpen(argv[1], 115200, AT_BIN | AT_BLOCKING);
	printf("fd %d\n", fd);

	if(fd > 0){
		printf("Opened\n");
		for(unsigned char i = 'a'; i <= 'z'; i++){
			atWrite(fd, &i, sizeof(unsigned char));
			usleep(1000);
		}
	}
	else{
		printf("Failed to open");
	}

	return 0;
}
