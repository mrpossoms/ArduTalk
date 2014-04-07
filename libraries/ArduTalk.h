#ifndef _ARDU_TALK
#define _ARDU_TALK
#include "Arduino.h"

class ArduTalk{
	public:
		ArduTalk(int baud);
		int Read(void* dst, int size);
		int Write(void* src, int size);
	private:
		int _AT_MSG_SIZE;
		int _decode(void* dst, int size, char* hex);
		int _encode(void* src, int size, char* hex);
};

#endif
