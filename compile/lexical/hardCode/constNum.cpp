#include "hardCode.h"
using namespace std;

char ConstNum(char ch, list<Token*>* tokenList) {
    // printf("in constnum.cpp %c\n", ch);
    int val = 0;
    if (ch != '0') {
        //10
        do {
            val = val * 10 + ch - '0';
            ch = scan();
        } while(ch >= '0' && ch <= '9');
    } else {
        ch = scan();
        if (ch == 'x' || ch == 'X') {
            //16
            ch = scan();
            if (ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F'
                || ch >= 'a' && ch <= 'f') {
                    //appear at least once => not error
                    do {
                        val = val * 16 + ch;
                        if (ch >= '0' && ch <= '9') val -= '0';
                        else if (ch >= 'a' && ch <= 'f') val -= 'a';
                        else if (ch >= 'A' && ch <= 'F') val -= 'A';
                        ch = scan();
                    } while (ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F'
                        || ch >= 'a' && ch <= 'f');
            } else {
                //error
                LEXERROR(NUM_HEX_TYPE);
                tokenList->push_back(new Token(ERR));
                ch = scan();
            }
        } else if (ch == 'b') {
            //2
            ch = scan();
            if (ch == '0' || ch == '1') {
                //appear at least once => not error
                do {
                    val = val * 2 + ch - '0';
                    ch = scan();
                } while(ch == '0' || ch == '1');
            } else {
                //error
                LEXERROR(NUM_BIN_TYPE);
                tokenList->push_back(new Token(ERR));
                ch = scan();
            }
        } else if (ch >= '0' && ch <= '7') {
            //8
            do {
                val = val * 8 + ch - '0';
                ch = scan();
            } while(ch >= '0' && ch <= '7');
        } else {
            //0
            ch = scan();
        }
    }
    tokenList->push_back(new Num(val));
    return ch;
}