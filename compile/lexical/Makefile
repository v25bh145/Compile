include $(PWD)/compile/lexical/errorHandler/Makefile
include $(PWD)/compile/lexical/hardCode/Makefile
include $(PWD)/compile/lexical/scanner/Makefile

cc = g++
lexicalObject = scanner.o errorHandler.o $(hardCodeObject)

lexical.o: $(lexicalObject) $(PWD)/compile/lexical/lexical.cpp
	$(cc) -c $(PWD)/compile/lexical/lexical.cpp