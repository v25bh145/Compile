/**
 * in book
 * */
#include <iostream>
#include <list>
#include "compile.h"
#include "lexical/lexical.h"
#include "lexical/scanner/scanner.h"
using namespace std;

int COMPILE(char* filePath) {
    list<Token*> tokenList;
    if(readFile(filePath) < 0) return -1;
    tokenList = lexical_analyze();

    
    // auto p = tokenList.begin();
    // printf("%s\n", (*p)->toString().c_str());

    for(auto p = tokenList.begin(); p != tokenList.end(); p++) {
        printf("%s\n", (*p)->toString().c_str());
    }
}