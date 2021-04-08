#include "hardCode.h"
using namespace std;

char ConstChar(char ch, list<Token*>* tokenList) {
    // printf("in constchar.cpp %c\n", ch);
    char c;
    ch = scan();
    //转义
    if(ch == '\\') {
        ch = scan();
        if(ch == 'n') c = '\n';
        else if (ch == '\\') c = '\\';
        else if (ch == 't') c = '\t';
        else if (ch == '"') c = '"';
        else if (ch == '0') c = '\0';
        else if(ch == -1 || ch == '\n') {
            //err
            LEXERROR(CHAR_NO_R_QUTION);
            tokenList->push_back(new Token(ERR));
        }
        //不进行\转义
        else c = ch;
        ch = scan();
    } else if(ch == '\n' || ch == -1) {
        //err
        LEXERROR(CHAR_NO_R_QUTION);
        tokenList->push_back(new Token(ERR));
        ch = scan();
    } else if (ch == '\'') {
        //no-data
        LEXERROR(CHAR_NO_DATA);
        tokenList->push_back(new Token(ERR));
        ch = scan();
    } else {
        c = ch;
        ch = scan();
    }
    //close
    if(ch == '\'') tokenList->push_back(new Char(c));
    else {
        //err
        LEXERROR(CHAR_NO_R_QUTION);
        tokenList->push_back(new Token(ERR));
    }
    ch = scan();
}