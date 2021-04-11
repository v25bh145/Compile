/**
 * 语义分析的主循环-扫描与整理
 **/
#include "parser.h"
#include "parserSign.h"
using namespace std;

Nonterminal *Parser::run() {
    auto programRes = program(parserRoot);
    return parserRoot;
}

void Parser::test() {
    printf("test\n");
}

void Parser::print(ParserSign  *node) {
    if(node->isTerminal) {
        printf("terminal: " + ((Terminal *)node)->getType());
    } else {

    }
}