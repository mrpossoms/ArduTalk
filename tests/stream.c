#include "ardutalk.h"

int main(){
	int fd = atOpen("/dev/tty.usbserial-FTGCHM6G", 57600);
	int bytes = 1024;
	atConfig(fd, AT_BIN | AT_NCHKSUM);

	while(1){
		char c = 0;
		usleep(1000);
		
		if(atRead(fd, &c, 1)){
			write(1, &c, 1);
		} 

		if(bytes == 0){
			atWrite(fd, "#ob", 3); // Turn on binary output
			atWrite(fd, "#o1", 3); // Turn on continuous streaming output
			atWrite(fd, "#oe0", 4); // Disable error message output
			atWrite(fd, "#s00", 4);
			bytes = 1024;
		}
		--bytes;
	}

	return 0;
}
