#include "hardCode.h"
using namespace std;

char Null(char ch, list<Token*>* tokenList) {
    // printf("in null.cpp %c\n", ch);
    while(ch == ' ' || ch == '\n' || ch == '\t') {
        ch = scan();
    }
    return ch;
}