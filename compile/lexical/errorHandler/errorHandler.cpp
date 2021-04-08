#include "../scanner/scanner.h"
#include "errorHandler.h"
#include <iostream>
using namespace std;

int errorNum = 0;
void lexError(int code) {
    static const char *lexErrorTable[] = {
        "字符串丢失右引号",
        "二进制数没有实体数据",
        "十六进制数没有实体数据",
        "字符丢失右单引号",
        "不支持空字符",
        "错误的\"或\"运算符",
        "多行注释没有正常结束",
        "词法记号不存在"
    };
    errorNum++;
    printf("%s<%d行，%d列> 词法错误：%s.\n", 
        getFilePath(),
        getLine(),
        getCol(),
        lexErrorTable[code]
    );
}
int getErrorNum() {
    return errorNum;
}