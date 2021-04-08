#include "hardCode.h"
using namespace std;

char ConstStr(char ch, list<Token*>* tokenList) {
    // printf("in conststr.cpp %c\n", ch);
    string str = "";
    ch = scan();
    while(ch != '"') {
        if(ch == '\\') {
            ch = scan();
            if(ch == 'n') str.push_back('\n');
            else if (ch == '\\') str.push_back('\\');
            else if (ch == 't') str.push_back('\t');
            else if (ch == '"') str.push_back('"');
            else if (ch == '0') str.push_back('\0'); 
            else if (ch == '\n');//connect
            else if(ch == -1) {
                //err
                LEXERROR(STR_NO_R_QUTION);
                tokenList->push_back(new Token(ERR));
                return ch;
            }
            //不进行\转义
            else str.push_back(ch);
        } else if(ch == '\n' || ch == -1) {
            LEXERROR(STR_NO_R_QUTION);
            //err
        } else //普通字符
            str.push_back(ch);
        ch = scan();
    }
    tokenList->push_back(new Str(str));
    ch = scan();
    return ch;
}