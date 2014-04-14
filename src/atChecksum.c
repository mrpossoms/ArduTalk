#include "atChecksum.h"
#include <stdio.h>

unsigned char atChecksum(void* data, size_t size){
	unsigned char chksum = 0;

	for(;size--;){
		chksum ^= ((unsigned char*)data)[size];
	}

	printf("atChecksum() - %02x\n", chksum);

	return chksum;
}