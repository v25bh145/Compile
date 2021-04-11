/**
 * 语义分析的主循环-扫描与整理
 **/
#include "parser.h"
#include "parserSign.h"
using namespace std;

Nonterminal *Parser::run() {
    matchInfo programRes = program(parserRoot);
    // cout<<programRes.info<<endl;
    return parserRoot;
}

void Parser::test() {
    printf("test\n");
}
//返回此节点递归包含的词法记号数
int Parser::print(ParserSign  *node, int stack) {
    if(node == NULL) {
        cout<<"terminal: NULL"<<endl;
        return 0;
    }
    if(node->isTerminal) {
        cout<<"terminal: " + ((Terminal *)node)->toString()<<endl;
        return 1;
    }
    int res = 0;
    
    Nonterminal *NonterminalNode = (Nonterminal *)node;

    cout<<NonterminalNode->toString()<<endl;

    list<ParserSign*> children = NonterminalNode->getChildren();
    for(auto p = children.begin(); p != children.end(); p++) {
        for(int i = 0; i < stack; i++) {
            cout<<">";
        }
        res += print(*p, stack + 1);
    }
    return res;
}