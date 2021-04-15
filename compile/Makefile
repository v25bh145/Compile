include $(PWD)/compile/lexical/Makefile
include $(PWD)/compile/parser/Makefile

cc = g++
compileObject = lexical.o $(lexicalObject) parserMain.o $(parserObject)

compile.o: $(compileObject) $(PWD)/compile/compile.cpp
	$(cc) -c $(PWD)/compile/compile.cpp