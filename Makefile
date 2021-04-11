include $(PWD)/compile/Makefile

cc = g++
mainObjects = $(compileObject) compile.o

main: $(mainObjects) main.o
	$(cc) main.o $(mainObjects) -o main

.PHONY: run
run: main
	./main test.cpp

.PHONY: target
target: main
	rm -rf *.o

.PHONY: clean
clean:
	rm -rf *.o
	rm -f main