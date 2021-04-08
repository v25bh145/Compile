#include <iostream>
#include <list>
#include "scanner/scanner.h"
#include "hardCode/hardCode.h"
#include "lexical.h"
using namespace std;

list<Token*> tokenList;
// Token* t;
Keywords keywords = Keywords();
string TagMap[48] = {
    "ERR", "END", 
    "ID", 
    "KW_INT", "KW_CHAR", "KW_VOID",
    "KW_EXTERN", 
    "NUM", "CH", "STR", 
    "NOT", "LEA", 
    "ADD", "SUB", "MUL", "DIV", "MOD", 
    "INC", "DEC", 
    "GT", "GE", "LT", "LE", "EQU", "NEQU", 
    "AND", "OR", 
    "LPAREN", "RPAREN",     
    "LBRACK", "RBRACK", 
    "LBRACE", "RBRACE", 
    "COMMA", "COLON", "SEMICON", 
    "ASSIGN", 
    "KW_IF", "KW_ELSE", 
    "KW_SWITCH", "KW_CASE", "KW_DEFAULT", 
    "KW_WHILE", "KW_DO", "KW_FOR", 
    "KW_BREAK", "KW_CONTINUE", "KW_RETURN"
};

list<Token*> lexical_analyze() {
    char ch = scan();
    while(ch != -1) {
        if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_')
            ch = Sign(ch, &tokenList);
        else if (ch >= '0' && ch <= '9')
            ch = ConstNum(ch, &tokenList);
        else if(ch == '\'')
            ch = ConstChar(ch, &tokenList);
        else if(ch == '"')
            ch = ConstStr(ch, &tokenList);
        else if(ch == ' ' || ch == '\n' || ch == '\t')
            ch = Null(ch, &tokenList);
        else ch = Bound(ch, &tokenList);
    }
    return tokenList;
}

string Token::toString() {
    return "tag:" + string(TagMap[tag]);
}