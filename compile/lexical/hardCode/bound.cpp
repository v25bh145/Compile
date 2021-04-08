#include "hardCode.h"
using namespace std;
class Token;
class Keywords;
class Id;
class Num;
class Char;
class Str;
char Bound(char ch, list<Token*>* tokenList) {
    // printf("in bound.cpp %c\n", ch);
    char next = scan();
    switch(ch) {
        case '+':
            if(next == '+') {
                next = scan();
                tokenList->push_back(new Token(INC));
            } else
                tokenList->push_back(new Token(ADD));
            break;
        case '-':
            if(next == '-') {
                next = scan();
                tokenList->push_back(new Token(DEC));
            } else
                tokenList->push_back(new Token(SUB));
            break;
        case '*':
            tokenList->push_back(new Token(MUL));
            break;
        case '/':
        //comment & div
            if(next == '/') {
                while(!(next == '\n' || next == -1))
                    next = scan();
                next = scan();
                // tokenList->push_back(new Token(ERR));
            } else if(next == '*') {
                next = scan();
                while(next != -1) {
                    if(next != '*'){ next = scan(); continue;}
                    while(next == '*') next = scan();
                    if(next == '/') {
                        // tokenList->push_back(new Token(ERR));
                        next = scan();
                        break;
                    }
                }
                if(next == -1) {
                    //coment no end
                    LEXERROR(COMMENT_NO_END);
                    tokenList->push_back(new Token(ERR));
                }
            } else
            //div
                tokenList->push_back(new Token(DIV));
            break;
        case '%':
            tokenList->push_back(new Token(MOD));
            break;
        case '>':
            if(next == '=') {
                next = scan();
                tokenList->push_back(new Token(GE));
            } else
                tokenList->push_back(new Token(GT));
            break;
        case '<':
            if(next == '=') {
                next = scan();
                tokenList->push_back(new Token(LE));
            } else
                tokenList->push_back(new Token(LT));
            break;
        case '=':
            if(next == '=') {
                next = scan();
                tokenList->push_back(new Token(EQU));
            } else
                tokenList->push_back(new Token(ASSIGN));
            break;
        case '&':
            if(next == '&') {
                next = scan();
                tokenList->push_back(new Token(AND));
            } else
                tokenList->push_back(new Token(LEA));
            break;
        case '|':
            if(next == '|') {
                //err
                LEXERROR(TOKEN_NO_EXIST);
                tokenList->push_back(new Token(ERR));
                next = scan();
            } else 
                tokenList->push_back(new Token(OR));
            break;
        case '!':
            if(next == '=') {
                next = scan();
                tokenList->push_back(new Token(NEQU));
            } else
                tokenList->push_back(new Token(NOT));
            break;
        case ',':
            tokenList->push_back(new Token(COMMA));
            break;
        case ':':
            tokenList->push_back(new Token(COLON));
            break;
        case ';':
            tokenList->push_back(new Token(SEMICON));
            break;
        case '(':
            tokenList->push_back(new Token(LPAREN));
            break;
        case ')':
            tokenList->push_back(new Token(RPAREN));
            break;
        case '[':
            tokenList->push_back(new Token(LBRACK));
            break;
        case ']':
            tokenList->push_back(new Token(RBRACK));
            break;
        case '{':
            tokenList->push_back(new Token(LBRACE));
            break;
        case '}':
            tokenList->push_back(new Token(RBRACE));
            break;
        case -1:
            tokenList->push_back(new Token(END));
            break;
        default:
            //err
            LEXERROR(TOKEN_NO_EXIST);
            tokenList->push_back(new Token(ERR));
    }
    return next;
}