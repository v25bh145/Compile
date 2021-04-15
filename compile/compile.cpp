/**
 * in book
 * */
#include <iostream>
#include <list>
#include "compile.h"
#include "lexical/lexical.h"
#include "lexical/scanner/scanner.h"
#include "parser/parserSign.h"
#include "parser/parser.h"
using namespace std;

int Compile(char* filePath) {
    list<Token*> tokenList;
    if(readFile(filePath) < 0) return -1;
    tokenList = lexical_analyze();

    cout<<"======词法分析======"<<endl;

    for(auto p = tokenList.begin(); p != tokenList.end(); p++) {
        printf("%s\n", (*p)->toString().c_str());
    }

    Parser* parser = new Parser(tokenList);
    Nonterminal *root = parser->run();

    cout<<"======词法数分析======"<<endl;
    parser->print(root, 0);
}