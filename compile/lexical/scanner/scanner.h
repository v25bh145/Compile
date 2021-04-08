#ifndef SCANNER_H
#define SCANNER_H
#define BUFLEN 80
using namespace std;

//once read a character
char scan();
int readFile(char* filename);
char* getFilePath();
int getLine();
int getCol();
#endif
