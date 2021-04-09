#include "hardCode.h"
using namespace std;

char Sign(char ch, list<Token*>* tokenList) {
    // printf("in sign.cpp %c\n", ch);
    Keywords keywords = Keywords();
    string name = "";
    do {
        name.push_back(ch);
        ch = scan();
    } while(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_' || ch >= '0' && ch <= '9');
    Tag tag = keywords.getTag(name);
    if (tag == ID) tokenList->push_back(new Id(name));
    else tokenList->push_back(new Token(tag));
    return ch;
}