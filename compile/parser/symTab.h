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
    GenIR* ir;
    void bindIr(GenIR* ir) {
        this->ir = ir;
    }
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
    //self
    //TODO impl
    void addInst(InterInst* inst);
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
    Var(vector<int> scopePath, bool isExtern, Tag tag, string id, int len);
    //变量/指针变量的构造函数
    Var(vector<int> scopePath, bool isExtern, Tag tag, bool isPtr, string id, Var* initVal);
    //常量的构造函数
    Var(Token* it);
    //TODO 中间变量的构造函数
    Var(vector<int> scopePath, Tag tag, bool isExtern);
    Var(vector<int> scopePath, Var* initVal);
    void setType(Tag t);
    void setArray(int len);
    bool setInit();
    static Var* getStep(Var* val) {
        // TODO SymTab::one SymTab::four
        // 基本类型（非指针）
        // if(val->isBase()) return SymTab::one;
        // char* 类型
        // else if(val->type == KW_CHAR) return SymTab::one;
        // int* 类型
        // else if(val->type == KW_INT)return SymTab::four;
        // else return NULL;
    }
    //self
    bool isVoid();
    bool isBase();
    bool isRef();
    static Var* getVoid();
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
    //目标代码
    vector<InterInst *> interCode;
    //返回点
    InterInst* returnPoint;
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
    //self
    // TODO impl
    InterInst* getReturnPoint();
};
//代码生成的辅助函数
class GenIR {
public:
    GenIR() {}
// TODO init file
    FILE* file;
    SymTab* symTab;
    void bindSymTab(SymTab* symTab) {
        this->symTab = symTab;
    }
    GenIR(SymTab* symTab);
    bool genVarInit(Var* var);
    void genReturn(Var* ret);
    void genFunHead(Fun* function);
    void genFunTail(Fun* function);
    Var* genPtr(Var* val);
    Var* genLea(Var* val);
    //操作为创建一个val的拷贝tmp，返回的值为tmp
    Var* genAssign(Var* val);
    //操作为lval = rval，返回lval
    Var* genAssign(Var* lval, Var* rval);
    bool typeCheck(Var* lval, Var* rval);
    static bool typeCheck(Var* lval, Var* rval) {
        bool flag = false;
        if(!rval) return false;
        if(lval->isBase() && rval->isBase()) flag = true;
        else if(!lval->isBase() && !rval->isBase()) flag = (lval->type == rval->type);
        return flag;
    }
    //
    Var* genTwoOp(Var* lval, Tag opt, Var* rval);
    Var* genOr(Var* lval, Var* rval);
    Var* genAnd(Var* lval, Var* rval);
    Var* genEqu(Var* lval, Var* rval);
    Var* genNequ(Var* lval, Var* rval);
    Var* genAdd(Var* lval, Var* rval);
    Var* genSub(Var* lval, Var* rval);
    Var* genGt(Var* lval, Var* rval);
    Var* genGe(Var* lval, Var* rval);
    Var* genLt(Var* lval, Var* rval);
    Var* genLe(Var* lval, Var* rval);
    Var* genMul(Var* lval, Var* rval);
    Var* genDiv(Var* lval, Var* rval);
    Var* genMod(Var* lval, Var* rval);
    Var* getStep(Var* v);
    //
    Var* genOneOpLeft(Tag opt, Var* val);
    Var* genIncL(Var* val);
    Var* genDecL(Var* val);
    Var* genNot(Var* val);
    Var* genMinus(Var* val);
    //
    Var* genOneOpRight(Var* val, Tag opt);
    Var* genIncR(Var* val);
    Var* genDecR(Var* val);
    // 
    Var* genArray(Var* array, Var* index);
    // 
    void genPara(Var* arg);
    Var* genCall(Fun* function, vector<Var*>& args);
};
enum Operator{
    OP_NOP,
    OP_DEC,
    OP_ENTRY, OP_EXIT,
    OP_AS,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_NEG,
    OP_GT, OP_GE, OP_LT, OP_LE, OP_EQU, OP_NE,
    OP_NOT,
    OP_AND, OP_OR,
    OP_LEA,
    OP_SET, OP_GET,
    OP_JMP,
    OP_JT, OP_JF, OP_JNE,
    OP_ARG,
    OP_PROC,
    OP_CALL,
    OP_RET,
    OP_RETV
};
//中间代码层-四元组
class InterInst {
public:
    string label;
    Operator op;
    Var* result;
    Var* arg1;
    Var* arg2;
    Fun* fun;
    InterInst* target;
    InterInst();
    InterInst(Operator op, Var* var);
    InterInst(Operator op, Fun* fun);
    InterInst(Operator op, Fun* fun, Var* ret);
    InterInst(Operator op, InterInst* interInst, Var* var);
    InterInst(Operator op, InterInst* interInst);
    InterInst(Operator op, Var* var1, Var* var2);
    // ???
    InterInst(Operator op, Var* var1, Var* var2, Var* var3);
};
#endif