/**
 * 符号表
 **/
#ifndef SYMTAB_H
#define SYMTAB_H
#include "../lexical/lexical.h"
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
class SymTab {
    public:
        struct string_hash {
            size_t operator() (const string& str) const {
                hash<string> hash_string;
                return hash_string(str.c_str());
                // return __gnu_cxx::__stl_hash_string(str.c_str());
            }
        };
        unordered_map<string, vector<Var*>*, string_hash> varTab;
        unordered_map<string, Var*, string_hash> strTab;
        unordered_map<string, Fun*, string_hash> funTab;
        //当前分析到的函数
        //如果无值，则默认NULL
        Fun* curFun;
        //作用域编号
        int scopeId;
        //作用域路径
        vector<int> scopePath;
        void enter();
        void leave();
};
//变量/常量
class Var {
    public:
        //是否为常量
        bool literal;
        //作用域路径
        vector<int> scopePath;
        //是否有extern关键字
        bool externed;
        //变量类型
        Tag type;
        //变量名称
        string name;
        //是否为指针
        bool isPtr;
        //是否为数组
        bool isArray;
        //数组长度
        int arraySize;
        //是否可以作为左值
        bool isLeft;
        //初始数据
        Var* initData;
        //是否初始化
        bool inited;
        //int char初值
        union{
            int intVal;
            char charVal;
        };
        //字符串常量初值
        string strVal;
        //字符指针初值
        string ptrVal;
        //变量的指针变量
        Var *ptr;
        //变量的大小
        int size;
        //变量的栈帧偏移
        int offset;
};
//函数
class Fun {
    public:
        //是否有extern关键字
        bool externed;
        //函数的返回类型
        Tag type;
        //函数名
        string name;
        //函数的形参变量列表
        vector<Var*> paraVar;
        //栈的最大深度
        int maxDepth;
        //当前栈指针位置
        int curEsp;
        //作用域指针位置
        vector<int> scopeEsp;
        /*TODO
        //目标代码
        vector<InterInst *> interCode;
        //返回点
        InterInst *returnPoint;
        */
    void enterScope();
};
#endif