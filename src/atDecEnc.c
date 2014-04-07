#include "atDecEnc.h"
#include <string.h>
#include <stdio.h>

int atEncode(void* src, size_t size, char* hex){
	int off = 0, i = 0;

	hex[0] = '$'; // set the start character
	for(i = 0; i < size; i++){
		sprintf(&hex[1 + (i << 1)], "%02x ", ((char*)src)[i]); 
	}
	hex[1 + (size << 1)] = '\n'; // set the newline

	return 0;
}

int atDecode(void* dst, size_t size, char* hex){
	int i = 0;

	for(; i < size; i++){
		sscanf(&hex[1 + (i << 1)], "%02x", &((unsigned char*)dst)[i]);
	}
	
	return 0;	
}
