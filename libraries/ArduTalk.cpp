#include "ArduTalk.h"
#define DEBUG

ArduTalk::ArduTalk(Stream* serial, int baud){
	_serial = serial;
}
//------------------------------------------------------
int ArduTalk::Read(void* dst, int size){
	int msgSize = size << 1, bytes = 0;
	char hex[msgSize];

	// wait for the beginning of a transmission
	_serial->readBytesUntil('$', hex, msgSize);
#ifdef DEBUG
	Serial.write(hex);
#endif

	// continue reading until expected data is recieved
	//do{
		// read to the end of line
		bytes = _serial->readBytesUntil('\n', hex, msgSize);
	//}while(bytes != msgSize);

	// decode the hex string back into binary
	_decode(dst, size, hex);

	// TODO checksum
	_serial->write((const uint8_t*)"!\n", 2); // send ack

	return 0;
}
//------------------------------------------------------
int ArduTalk::Write(void* src, int size){
	int msgSize = 1 + (size << 1);
	char hex[msgSize], ack[1];

	// encode binary into hex ascii string
	_encode(src, size, hex);

	//do{
#ifdef DEBUG
		Serial.println("Writing");
		Serial.write(hex);
#endif
		// send the message
		_serial->write(hex);

		// wait for ack
		_serial->readBytesUntil('\n', ack, 1);
	//}while(ack[0] != '!');

	return 0;
}
//------------------------------------------------------
int ArduTalk::_decode(void* dst, int size, char* hex){
	for(int i = 0; i < size; i++){
		sscanf(&hex[i << 1], "%02x", ((unsigned char*)dst) + i);
	}

	return 0;
}
//------------------------------------------------------
int ArduTalk::_encode(void* src, int size, char* hex){
	int off = 0, i = 0;

	hex[0] = '$'; // set the start character
	for(i = 0; i < size; i++){
		sprintf(&hex[1 + (i << 1)], "%02x ", ((char*)src)[i]); 
	}
	hex[1 + (size << 1)] = '\n'; // set the newline
	
	return 0;
}
