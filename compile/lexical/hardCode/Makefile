cc = g++
hardCodeCpp = bound.cpp constStr.cpp constChar.cpp constNum.cpp null.cpp sign.cpp
hardCodeObject = bound.o constStr.o constChar.o constNum.o null.o sign.o
hardCodePath = $(PWD)/compile/lexical/hardCode

constChar.o: $(hardCodePath)/constChar.cpp
	$(cc) -c $(hardCodePath)/constChar.cpp

constStr.o: $(hardCodePath)/constStr.cpp
	$(cc) -c $(hardCodePath)/constStr.cpp

constNum.o: $(hardCodePath)/constNum.cpp
	$(cc) -c $(hardCodePath)/constNum.cpp

null.o: $(hardCodePath)/null.cpp
	$(cc) -c $(hardCodePath)/null.cpp

sign.o: $(hardCodePath)/sign.cpp
	$(cc) -c $(hardCodePath)/sign.cpp

bound.o: $(hardCodePath)/bound.cpp
	$(cc) -c $(hardCodePath)/bound.cpp