FLAGS=-Wint-to-pointer-cast
SRC=./src/*.c
INC=./include
DST=./lib
LINK=-string
LIB=libardutalk.a

TSTINC=./tests/include
TST=./tests
TSTS=./tests/*.c

CC=gcc

all:
	$(CC) -I$(INC) -c $(SRC)
		ar rcs $(DST)/$(LIB) *.o
			rm *.o

testlib: $(SRC)
	$(CC) -I$(INC) -I$(TSTINC) -c $(SRC)
	mkdir -p $(TST)/lib
	ar rcs $(TST)/lib/$(LIB) *.o
	rm *.o

tests: testlib
	$(foreach test, $(TSTS), echo $(test);)
	$(CC) -I$(INC) $(SRC) $(TST)/decEnc.c -o $(TST)/bin/decEnc.bin
	$(CC) -I$(INC) $(SRC) $(TST)/write.c -o $(TST)/bin/write.bin
	$(CC) -I$(INC) $(SRC) $(TST)/radioControl.c -o $(TST)/bin/radioControl.bin
	$(CC) -I$(INC) $(SRC) $(TST)/stream.c -o $(TST)/bin/stream.bin
	$(CC) -I$(INC) $(SRC) $(TST)/read.c -o $(TST)/bin/read.bin
#gcc -L$(TST)/lib -I$(INC) $(test) -o $(TST)/bin/$(test).bin -lArduTalk;)
package:
	make
	mkdir -p $(DST)/include
	cat /dev/null >| $(DST)/include/ardutalk.h
	find ./$(INC) -name "*.h" -exec cat {} > $(DST)/include/ardutalk.h \;
	
install:
	echo 'Installing the library'
	cp $(DST)/include/*.h /usr/include
	cp $(DST)/*.a /usr/lib
clean:
	find ./ -iname "*.a" -exec rm {} \;
	find ./ -iname "*.o" -exec rm {} \;
	find ./ -iname "*.out" -exec rm {} \;
setup:
	echo 'Setting up project'
	mkdir $(TST) $(DST) $(INC) ./src
	mkdir $(TSTINC)
