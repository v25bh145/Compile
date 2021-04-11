cc = g++
parserObject = parserMatcher.o parserScanner.o nonterminalMap.o
parserPath = $(PWD)/compile/parser

parserMain.o: $(parserObject) $(parserPath)/parserMain.cpp
	$(cc) -c $(parserPath)/parserMain.cpp

parserMatcher.o: $(parserPath)/parserMatcher.cpp
	$(cc) -c $(parserPath)/parserMatcher.cpp

parserScanner.o: $(parserPath)/parserScanner.cpp
	$(cc) -c $(parserPath)/parserScanner.cpp

nonterminalMap.o: $(parserPath)/nonterminalMap.cpp
	$(cc) -c $(parserPath)/nonterminalMap.cpp