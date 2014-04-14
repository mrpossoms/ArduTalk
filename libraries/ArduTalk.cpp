#include "ArduTalk.h"
//#define DEBUG

#define START_SYMBOL 1
#define NEW_LINE     1
#define CHECKSUM     1

ArduTalk::ArduTalk(Stream* serial, int baud){
	_serial = serial;
}
//------------------------------------------------------
int ArduTalk::Read(void* dst, int size){
	int bytes = 0;
	int msgSize = ((size + CHECKSUM) << 1);
	char hex[msgSize], _read[size + CHECKSUM];

	// wait for the beginning of a transmission
	_serial->readBytesUntil('$', hex, msgSize);

#ifdef DEBUG
	Serial.println("Read start char");
	Serial.print("Reading "); Serial.print(msgSize, DEC); Serial.println(" bytes");
#endif

	// read to the termination character
	bytes = _serial->readBytesUntil('\n', hex, msgSize);
	_decode(_read, size + CHECKSUM, hex);

#ifdef DEBUG
	Serial.write((const uint8_t*)hex, msgSize);
#endif

	if(_read[size] != _checksum(_read, size)){
		// this message is corrupted
#ifdef DEBUG
		Serial.println('bad checksum');
#endif
		return -1;
	}

#ifdef DEBUG
	Serial.println("checksum ok!");
#endif

	// The checksums match, we are good
	memcpy(dst, _read, size);

	return bytes;
}
//------------------------------------------------------
int ArduTalk::Write(void* src, int size){
	int msgSize = ((size + CHECKSUM) << 1) + START_SYMBOL + NEW_LINE;
	char hex[msgSize + 1], _src[size + CHECKSUM];

	memset(hex, 0, msgSize + 1);

	// encode, and write the message
	memcpy(_src, src, size);
	_src[size] = _checksum(src, size);
	_encode(_src, size + CHECKSUM, hex);

	// send the message
	_serial->write((const uint8_t*)hex, msgSize);

#ifdef DEBUG
	Serial.print("Writing "); Serial.println(msgSize, DEC);
	Serial.print(hex);
#endif
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
//------------------------------------------------------
unsigned char ArduTalk::_checksum(void* data, int size){
	unsigned char chksum = 0;

	for(;size--;){
		chksum ^= ((unsigned char*)data)[size];
	}

	return chksum;
}