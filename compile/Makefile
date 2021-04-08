include $(PWD)/compile/lexical/Makefile
include $(PWD)/compile/parser/Makefile

cc = g++
compileObject = lexical.o $(lexicalObject) parserMain.o $(parserObjeect)

compile.o: $(compileObject)
	$(cc) -c $(PWD)/compile/compile.cpp