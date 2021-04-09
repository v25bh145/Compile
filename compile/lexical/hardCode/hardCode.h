#ifndef HARDCODE_H
#define HARDCODE_H
#include "../scanner/scanner.h"
#include "../lexical.h"
#include "../errorHandler/errorHandler.h"
#include <iostream>
#include <list>
using namespace std;
char Sign(char ch, list<Token*>* tokenList);
char ConstNum(char ch, list<Token*>* tokenList);
char ConstChar(char ch, list<Token*>* tokenList);
char ConstStr(char ch, list<Token*>* tokenList);
char Bound(char ch, list<Token*>* tokenList);
char Null(char ch, list<Token*>* tokenList);
#endif