/**
 * 语义分析的扫描器
 **/
#include "parser.h"
using namespace std;
bool Parser::move() { 
    tokenIterator++;
    if(tokenIterator == this->tokenList.end()) return false;
    return true;
}
Token* Parser::scan() {
    if(tokenIterator == this->tokenList.end()) return NULL;
    return *tokenIterator;
}