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

class SymTab;
class Var;
class Fun;
class InterInst;
class GenIR;

class SymTab {
public:
    SymTab();
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
    void bindIr(GenIR* ir);
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
    // self
    void addInst(InterInst* inst);
    // self
    vector<InterInst*> instList;
    static Var* one;
    static Var* four;
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
    //中间变量的构造函数
    Var(vector<int> scopePath, Tag tag, bool isExtern);
    Var(vector<int> scopePath, Var* initVal);
    void setType(Tag t);
    void setArray(int len);
    bool setInit();
    static Var* getStep(Var* val);
    //self
    bool isVoid();
    bool isBase();
    bool isRef();
    static Var* getVoid();
    static Var* getTrue();
    Var();
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
    Fun(bool isExtern, Tag tag, string name, vector<Var*> paraList);
};
//代码生成的辅助函数
class GenIR {
public:
    static int lbNum;
    GenIR();
// TODO init file
    FILE* file;
    SymTab* symTab;
    void bindSymTab(SymTab* symTab);
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
    // bool typeCheck(Var* lval, Var* rval);
    static bool typeCheck(Var* lval, Var* rval);
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
    //
    // 产生else标签
    // OP_JF cond _else
    void genIfHead(Var* cond, InterInst*& _else);
    // 无else
    // _else:
    void genIfTail(InterInst*& _else);
    // 有else
    // 产生exit标签
    // OP_JMP _exit
    // _else:
    void genElseHead(InterInst* _else, InterInst*& _exit);
    // _exit:
    void genElseTail(InterInst*& _exit);
    //
    static string genLb();
    // 
    // 产生_exit标签，为其内部的break提供信息
    void genSwitchHead(InterInst*& _exit);
    // OP_JMP _exit
    void genSwitchTail(InterInst*& _exit);
    // 产生_case_exit标签，为其内部的break提供信息
    // OP_JNE cond lb _case_exit
    void genCaseHead(Var* cond, Var* lb, InterInst*& _case_exit);
    // _case_exit:
    void genCaseTail(InterInst* _case_exit);
    // 
    // 中间的部分需要用到_exit-注册、放置_while，注册_exit
    void genWhileHead(InterInst*& _while, InterInst* _exit);
    // 为false跳转到结尾，否则执行genWhileTail，跳转到开头
    void genWhileCond(Var* cond, InterInst* _exit);
    void genWhileTail(InterInst*& _while, InterInst*& _exit);
    // 
    void genDoWhileHead(InterInst*& _do, InterInst* _exit);
    // 为true跳转到开头
    void genDoWhileTail(Var* cond, InterInst*& _do, InterInst*& _exit);
    // 
    void genForHead(InterInst*& _for, InterInst*& _exit);
    void genForCondBegin(Var* cond, InterInst*& _step, InterInst*& _block, InterInst* _exit);
    void genForCondEnd(InterInst* _for, InterInst* _block);
    void genForTail(InterInst*& _step, InterInst*& _exit);
    // 
    vector<InterInst*> heads;
    vector<InterInst*> tails;
    void push(InterInst* head, InterInst* tail);
    void pop();
    void genBreak();
    void genContinue();
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
    // 标签
    string label;
    Operator op;
    Var* result;
    Var* arg1;
    Var* arg2;
    Fun* fun;
    InterInst* target;
    //生成一个随机的标签
    InterInst();
    InterInst(Operator op, Var* var);
    InterInst(Operator op, Fun* fun);
    InterInst(Operator op, Fun* fun, Var* ret);
    InterInst(Operator op, InterInst* interInst, Var* var2, Var* var3);
    InterInst(Operator op, InterInst* interInst, Var* var);
    InterInst(Operator op, InterInst* interInst);
    InterInst(Operator op, Var* var1, Var* var2);
    // ???
    InterInst(Operator op, Var* var1, Var* var2, Var* var3);
};
#endif