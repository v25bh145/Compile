#include <iostream>
#include "scanner.h"
using namespace std;

char* fp;
FILE* file = NULL;
int fileSize = 0;

int lineLen = 0;
int readPos = -1;
char line[BUFLEN];
//the present line in the content file
char lineNum = 1;
//the present col in the content file
int colNum = 0;
char lastCh = 0;

char* getFilePath() {
    return fp;
}

int getLine() {
    return lineNum;
}

int getCol() {
    return colNum;
}

char scan() {
    if(!file) return -1;
    //cache - line
    if(readPos == lineLen - 1) {
        lineLen = fread(line, 1, BUFLEN, file);
        //END
        if(lineLen == 0) {
            lineLen = 1;
            line[0] = -1;
        }
        readPos = -1;
    }
    //read a character
    readPos++;
    char ch = line[readPos];
    if(lastCh == '\n') {
        lineNum++;
        colNum = 0;
    }
    //END
    if(ch == -1) {
        fclose(file);
        file = NULL;
    }
    else if(ch != '\n') colNum++;
    lastCh = ch;
    return ch;
}

int readFile(char* filePath) {
    fp = filePath;
    if((file = fopen(filePath, "r")) == NULL) {
        printf("打开文件%s错误\n",  filePath);
        return -1;
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);//通过ftell函数获得指针到文件头的偏移字节数。
    rewind(file);
    return fileSize;
}