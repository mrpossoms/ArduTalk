#include "ardutalk.h"
#include <stdio.h>
#include <strings.h>

typedef struct{
	unsigned char rotors[4];
//	float delta[3];
//	float thrust;
} basStnMsg;

unsigned char computeChecksum(void* msg, int size){
	int chksum = 0;
	--size;

	for(;size--;){
		chksum ^= ((unsigned char*)msg)[size];
	}

	return chksum;
}

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

	int fd = atOpen("/dev/cu.usbserial-A702LY4I", 9600, 0);
	printf("fd %d\n", fd);

	if(fd > 0){
		int i = 10;
		printf("Opened\n");
		while(1){
			int j = 0, speed = 0;

			scanf("%d", &speed);

			for(j = 4; j--;){
				t.rotors[j] = speed;
			}
			
			atWrite(fd, &t, sizeof(basStnMsg));
		
			// clear it out, and read the response
			bzero(&t, sizeof(basStnMsg));
			//usleep(500000);
			atRead(fd, &t, sizeof(basStnMsg));

			for(j = 0; j < 4; j++){
				printf("Rotor[%d]=%d\n", j, t.rotors[j]);
			}
		}
	}
	else{
		printf("Failed to open");
	}

	printf("Done!\n");

	return 0;
}
