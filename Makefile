include $(PWD)/compile/Makefile

cc = g++
mainObjects = $(compileObject) compile.o

target : $(mainObjects) main.o
	$(cc) main.o $(mainObjects) -o main
	./main test.cpp

.PHONY: clean
clean:
	rm -rf *.o
	rm -f main