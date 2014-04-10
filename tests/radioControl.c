#include "atOpen.h"
#include "atWrite.h"
#include "atRead.h"

typedef struct{
	unsigned char rotors[4];
	unsigned char checksum;
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
	t.checksum = computeChecksum(&t, sizeof(basStnMsg) - 1);

	int fd = atOpen("/dev/cu.usbserial-A702LY4I", 9600);
	printf("fd %d\n", fd);

	if(fd > 0){
		int i = 10;
		printf("Opened\n");
		for(;i--;){
			atWrite(fd, &t, sizeof(basStnMsg));
			//usleep(50000);
		
			// clear it out, and read the response
			//bzero(&t, sizeof(basStnMsg));
			//atRead(fd, &t, sizeof(basStnMsg));
		
			for(i = 0; i < sizeof(basStnMsg); i++){
				printf("%02x ", ((char*)&t)[i]);
			}
		}
	}
	else{
		printf("Failed to open");
	}

	printf("Done!\n");

	return 0;
}
