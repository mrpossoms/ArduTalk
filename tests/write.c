#include "atOpen.h"
#include "atWrite.h"

typedef struct{
	int a;
	float b;
} test;

int main(){
	test t = {
		1337,
		13.37f
	};
	int fd = atOpen("./dump.dat", 9600);

	if(fd > 0){
		printf("Opened\n");
		atWrite(fd, &t, sizeof(test));
	}
	else{
		printf("Failed to open");
	}

	printf("%d\n%f", t.a, t.b);

	return 0;
}
