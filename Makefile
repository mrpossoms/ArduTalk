FLAGS=-Wint-to-pointer-cast
SRC=./src/*.c
INC=./include
DST=./lib
LINK=-string
LIB=libArduTalk.a

TSTINC=./tests/include
TST=./tests
TSTS=./tests/*.c

all:
	gcc -I$(INC) -c $(SRC)
		ar rcs $(DST)/$(LIB) *.o
			rm *.o

testlib: $(SRC)
	gcc -I$(INC) -I$(TSTINC) -c $(SRC)
		ar rcs $(TST)/lib/$(LIB) *.o
			rm *.o

tests: testlib
	$(foreach test, $(TSTS), echo $(test);)
	gcc -L$(TST)/lib -I$(INC) $(TST)/decEnc.c -o $(TST)/bin/decEnc.bin -lArduTalk
	gcc -L$(TST)/lib -I$(INC) $(TST)/write.c -o $(TST)/bin/write.bin -lArduTalk
	#gcc -L$(TST)/lib -I$(INC) $(test) -o $(TST)/bin/$(test).bin -lArduTalk;)
install:
	echo 'This will install the library'
clean:
	find ./ -iname "*.a" -exec rm {} \;
		find ./ -iname "*.o" -exec rm {} \;
			find ./ -iname "*.out" -exec rm {} \;
setup:
	echo 'Setting up project'
	mkdir $(TST) $(DST) $(INC) ./src
	mkdir $(TSTINC)
