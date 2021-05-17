#include <iostream>
#include "semError.h"
using namespace std;
int semErrorNum = 0, warnNum = 0;
void semError(int code, string name) {
    const char* semErrorTable[] = {
        "变量重定义",
        "函数重定义",
        "变量未声明",
        "函数未声明",
        "函数声明与定义不匹配",
        "函数形参与实参不匹配",
        "变量声明时不允许初始化",
        "函数定义不能使用声明保留字extern",
        "数组长度应该是正整数",
        "变量初始化类型错误",
        "全局变量初始化值不是常量",
        "变量不能声明为void类型",
        "无效的左值表达式",
        "赋值表达式类型不兼容",
        "表达式操作数不能是基本类型",
        "表达式操作数不是基本类型",
        "数组索引运算类型错误",
        "void的函数返回值不能参与表达式运算",
        "break语句不能出现在循环或switch语句之外",
        "continue不能出现在函数之外",
        "return语句和函数返回值类型不匹配"
    };
    semErrorNum++;
    printf("语义错误: %s %s.\n", name.c_str(), semErrorTable[code]);
}
void semWarn(int code, string name) {
    const char* semWarnTable[] = {
        "函数参数列表类型冲突",
        "函数返回值类型不精确匹配"
    };
    warnNum++;
    printf("语义错误: %s %s.\n", name.c_str(), semWarnTable[code]);
}