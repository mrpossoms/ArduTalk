#include "ardutalk.h"
#include <stdio.h>

typedef struct{
	int a;
	float b;
} test;

int main(){
	char enc[128];

	test t = {
		1337,
		13.37f
	};

	atEncode(&t, sizeof(test), enc);
	printf("%s", enc);
	atDecode(&t, sizeof(test), enc);

	printf("%d\n%f", t.a, t.b);

	return 0;
}
