#include <iostream>
#include <list>
#include "scanner/scanner.h"
#include "hardCode/hardCode.h"
#include "lexical.h"
using namespace std;

list<Token*> tokenList;
// Token* t;
Keywords keywords = Keywords();

list<Token*> lexical_analyze() {
    initTagMap();
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
    return "tag:" + getTagMap(tag);
}
string *TagMap;
void initTagMap() {
    TagMap = new string[50];
    TagMap[ERR] = "ERR";
    TagMap[END] = "END";
    TagMap[ID] = "ID";
    TagMap[KW_INT] = "KW_INT";
    TagMap[KW_CHAR] = "KW_CHAR";
    TagMap[KW_VOID] = "KW_VOID";
    TagMap[KW_EXTERN] = "KW_EXTERN";
    TagMap[NUM] = "NUM";
    TagMap[CH] = "CH";
    TagMap[STR] = "STR";
    TagMap[NOT] = "NOT";
    TagMap[LEA] = "LEA";
    TagMap[ADD] = "ADD";
    TagMap[SUB] = "SUB";
    TagMap[MUL] = "MUL";
    TagMap[DIV] = "DIV";
    TagMap[MOD] = "MOD";
    TagMap[INC] = "INC";
    TagMap[DEC] = "DEC";
    TagMap[GT] = "GT";
    TagMap[GE] = "GE";
    TagMap[LT] = "LT";
    TagMap[LE] = "LE";
    TagMap[EQU] = "EQU";
    TagMap[NEQU] = "NEQU";
    TagMap[AND] = "AND";
    TagMap[OR] = "OR";
    TagMap[LPAREN] = "LPAREN";
    TagMap[RPAREN] = "RPAREN";
    TagMap[LBRACK] = "LBRACK";
    TagMap[RBRACK] = "RBRACK";
    TagMap[LBRACE] = "LBRACE";
    TagMap[RBRACE] = "RBRACE";
    TagMap[COMMA] = "COMMA";
    TagMap[COLON] = "COLON";
    TagMap[SEMICON] = "SEMICON";
    TagMap[ASSIGN] = "ASSIGN";
    TagMap[KW_IF] = "KW_IF";
    TagMap[KW_ELSE] = "KW_ELSE";
    TagMap[KW_SWITCH] = "KW_SWITCH";
    TagMap[KW_CASE] = "KW_CASE";
    TagMap[KW_DEFAULT] = "KW_DEFAULT";
    TagMap[KW_WHILE] = "KW_WHILE";
    TagMap[KW_DO] = "KW_DO";
    TagMap[KW_FOR] = "KW_FOR";
    TagMap[KW_BREAK] = "KW_BREAK";
    TagMap[KW_CONTINUE] = "KW_CONTINUE";
    TagMap[KW_RETURN] = "KW_RETURN";
}
string getTagMap(int i) {
    return TagMap[i];
}