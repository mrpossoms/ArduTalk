#include "ArduTalk.h"

ArduTalk::ArduTalk(Stream* serial, int baud){
	_serial = serial;
}
//------------------------------------------------------
int ArduTalk::Read(void* dst, int size){
	int msgSize = size << 1, bytes = 0;
	char hex[msgSize];

	// wait for the beginning of a transmission
	_serial->readBytesUntil('$', hex, msgSize);
	Serial.write(hex, msgSize);

	// continue reading until expected data is recieved
	do{
		// read to the end of line
		bytes = _serial->readBytesUntil('\n', hex, msgSize);
	}while(bytes != msgSize);

	// decode the hex string back into binary
	_decode(dst, size, hex);

	// TODO checksum
	_serial->println("!"); // send ack

	return 0;
}
//------------------------------------------------------
int ArduTalk::Write(void* src, int size){
	int msgSize = 1 + (size << 1);
	char hex[msgSize], ack[1];

	// encode binary into hex ascii string
	_encode(src, size, hex);

	do{
		// send the message
		_serial->println(hex);

		// wait for ack
		_serial->readBytesUntil('\n', ack, 1);
	}while(ack[0] != '!');

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
