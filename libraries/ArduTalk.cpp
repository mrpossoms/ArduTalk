#include "ArduTalk.h"

ArduTalk::ArduTalk(int baud){
	Serial.begin(baud);
}
//------------------------------------------------------
int ArduTalk::Read(void* dst, int size){
	return 0;
}
//------------------------------------------------------
int ArduTalk::Write(void* src, int size){
	return 0;
}
//------------------------------------------------------
int ArduTalk::_decode(void* dst, int size, char* hex){
	return 0;
}
//------------------------------------------------------
int ArduTalk::_encode(void* src, int size, char* hex){
	return 0;
}
