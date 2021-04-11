cc = g++
parserObject = parserMatcher.o parserScanner.o nonterminalMap.o
parserPath = $(PWD)/compile/parser

parserMain.o: $(parserObject)
	$(cc) -c $(parserPath)/parserMain.cpp

parserMatcher.o:
	$(cc) -c $(parserPath)/parserMatcher.cpp

parserScanner.o:
	$(cc) -c $(parserPath)/parserScanner.cpp

nonterminalMap.o
	$(cc) -c $(parserPath)/nonterminalMap.cpp