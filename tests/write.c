#include "ardutalk.h"
#include <unistd.h>
#include <stdio.h>

typedef struct{
	unsigned char rotors[4];
//	float delta[3];
//	float thrust;
} basStnMsg;

int main(){
	/*
	basStnMsg t = {
		{0},
		13.37f
	};

	t.delta[0] = 100.0f;
	t.delta[1] = 33.0f;
	t.delta[2] = 0.7f;
	*/
	basStnMsg t;
	t.rotors[0] = 8;
	t.rotors[1] = 16;
	t.rotors[2] = 32;
	t.rotors[3] = 64;

	int fd = atOpen("/dev/cu.usbserial-A702LY4I", 9600);
	printf("fd %d\n", fd);

	if(fd > 0){
		int i = 10;
		printf("Opened\n");
		for(;i--;){
			atWrite(fd, &t, sizeof(basStnMsg));
			usleep(50000);
		}
	}
	else{
		printf("Failed to open");
	}


	return 0;
}
