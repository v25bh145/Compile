cc = g++
parserObject = parserMatcher.o parserScanner.o nonterminalMap.o symTab.o genIR.o interInst.o semError.o
parserPath = $(PWD)/compile/parser

parserMain.o: $(parserObject) $(parserPath)/parserMain.cpp
	$(cc) -c $(parserPath)/parserMain.cpp

parserMatcher.o: $(parserPath)/parserMatcher.cpp
	$(cc) -c $(parserPath)/parserMatcher.cpp

parserScanner.o: $(parserPath)/parserScanner.cpp
	$(cc) -c $(parserPath)/parserScanner.cpp

nonterminalMap.o: $(parserPath)/nonterminalMap.cpp
	$(cc) -c $(parserPath)/nonterminalMap.cpp

symTab.o: $(parserPath)/symTab.cpp
	$(cc) -c $(parserPath)/symTab.cpp

genIR.o: $(parserPath)/genIR.cpp
	$(cc) -c $(parserPath)/genIR.cpp

interInst.o: $(parserPath)/interInst.cpp
	$(cc) -c $(parserPath)/interInst.cpp

semError.o: $(parserPath)/semError.cpp
	$(cc) -c $(parserPath)/semError.cpp