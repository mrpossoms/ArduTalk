#include "atOpen.h"
#include "atWrite.h"

typedef struct{
	float delta[3];
	float thrust;
} basStnMsg;

int main(){
	basStnMsg t = {
		{0},
		13.37f
	};

	t.delta[0] = 100.0f;
	t.delta[1] = 33.0f;
	t.delta[2] = 0.7f;

	int fd = atOpen("/dev/cu.usbserial-A702LY4I", 9600);
	printf("fd %d\n", fd);

	if(fd > 0){
		int i = 1000;
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
