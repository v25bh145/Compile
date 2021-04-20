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
        //ir
        GenIR ir;
        //作用域路径
        vector<int> scopePath;
        void enter();
        void leave();
        void addVar(Var* var);
        void addStr(Var* &var);
        Var* getVar(string name);
        //声明
        void decFun(Fun* fun);
        //定义
        void defFun(Fun* fun);
        void endDefFun();
        Fun* getFun(string name, vector<Var*>& args);
};
//变量/常量
class Var {
    public:
        //是否为常量
        bool isLiteral;
        //作用域路径
        vector<int> scopePath;
        //是否有extern关键字
        bool isExtern;
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
        bool isInit;
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
        //数组的构造函数
        Var(vector<int> scopePath, bool isExtern, Tag tag, string id, int len) {
            this->scopePath = scopePath;
            this->isExtern = isExtern;
            this->setType(tag);
            this->name = id;
            this->setArray(len);
        }
        //变量/指针变量的构造函数
        Var(vector<int> scopePath, bool isExtern, Tag tag, bool isPtr, string id, Var* initVal) {
            this->scopePath = scopePath;
            this->isExtern = isExtern;
            this->setType(tag);
            this->isPtr = isPtr;
            this->name = id;
            this->initData = initVal;
        }
        //常量的构造函数
        Var(Token* it) {
            // clear(); ??
            this->isLiteral = true;
            this->isLeft = false;
            switch(it->tag) {
                case NUM: {
                    this->setType(KW_INT);
                    this->name = "<int>";
                    this->intVal = ((Num*)it)->val;
                    break;
                }
                case CH: {
                    this->setType(KW_CHAR);
                    this->name = "<char>";
                    this->intVal = 0;
                    this->charVal = ((Char*)it)->ch;
                    break;
                }
                case STR: {
                    this->setType(KW_CHAR);
                    //TODO GenCode
                    // this->name = GenCode::genLb();
                    this->strVal = ((Str*)it)->str;
                    this->setArray(this->strVal.size() + 1);
                    break;
                }
            }
        }
        void Var::setType(Tag t);
        void Var::setArray(int len);
        bool Var::setInit();
};
//函数
class Fun {
    public:
        //是否有extern关键字，注extern在这里是声明（dec）的意思
        //isExtern = true: 函数只有声明
        //isExtern = false: 函数有定义
        bool isExtern;
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
        void leaveScope();
        void locate(Var* var);
        bool match(Fun* fun);
        bool match(vector<Var*>& args);
        //这个函数的定义(当只有声明体的函数碰到定义时调用)
        void define(Fun* def);
        Fun(bool isExtern, Tag tag, string name, vector<Var*> paraList) {
            this->isExtern = isExtern;
            this->type = tag;
            this->name = name;
            this->paraVar = paraVar;
        }
};
//代码生成的辅助函数
class GenIR {
    bool genVarInit(Var* var);
    Var* genOneOpRight(Var* val, Tag opt);
    Var* genArray(Var* array, Var* index);
    void genReturn(Var* ret);
};
#endif