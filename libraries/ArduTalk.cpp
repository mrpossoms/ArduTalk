#include "ArduTalk.h"

ArduTalk::ArduTalk(int baud){
	Serial.begin(baud);
}
//------------------------------------------------------
int ArduTalk::Read(void* dst, int size){
	int msgSize = 1 + (size << 1);
	char hex[msgSize];

	// wait for the beginning of a transmission
	Serial.readBytesUntil('$', hex, msgSize);

	// read to the end of line
	Serial.readBytesUntil('\n', hex, msgSize);

	// decode the hex string back into binary
	_decode(dst, size, hex);

	// TODO checksum
	Serial.println("!"); // send ack

	return 0;
}
//------------------------------------------------------
int ArduTalk::Write(void* src, int size){
	int msgSize = 1 + (size << 1);
	char hex[msgSize];

	// encode binary into hex ascii string
	_encode(src, size, hex);

	// send the message
	Serial.println(hex);

	Serial.readBytesUntil('\n', hex, 1);
	if(hex[0] != '!'){
		// TODO this is an error
		return -1;
	}

	return 0;
}
//------------------------------------------------------
int ArduTalk::_decode(void* dst, int size, char* hex){
	for(int i = 0; i < size; i++){
		sscanf(&hex[i << 1], "%02x", ((uint8_t*)dst) + i);
	}

	return 0;
}
//------------------------------------------------------
int ArduTalk::_encode(void* src, int size, char* hex){
	int off = 0, i = 0;

	hex[0] = '$'; // set the start character
	for(i = 0; i < size; i++){
		sprintf(&hex[1 + (i << 1)], "%02x ", ((uint8_t*)src) + i); 
	}
	
	return 0;
}
