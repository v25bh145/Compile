include $(PWD)/compile/lexical/Makefile
include $(PWD)/compile/parser/Makefile
include $(PWD)/compile/generateX86/Makefile

cc = g++
compileObject = lexical.o $(lexicalObject) parserMain.o $(parserObject) generateX86.o

compile.o: $(compileObject) $(PWD)/compile/compile.cpp
	$(cc) -c $(PWD)/compile/compile.cpp