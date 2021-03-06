#include "symTab.h"
#include "semError.h"
#include <sstream>
using namespace std;
SymTab::SymTab() {
    curFun = NULL;
    scopeId = 0;
}
void SymTab::bindIr(GenIR* ir) {
    this->ir = ir;
}
void SymTab::enter() {
    scopeId++;
    scopePath.push_back(scopeId);
    //如果当前在函数作用域内
    if(curFun) curFun->enterScope();
}
void SymTab::leave() {
    scopePath.pop_back();
    if(curFun) curFun->leaveScope();
}
//添加变量/常量(不包含字串常量)
void SymTab::addVar(Var* var) {
    if(varTab.find(var->name) == varTab.end()) {
        //不同名变量
        varTab[var->name] = new vector<Var*>;
        varTab[var->name]->push_back(var);
    } else {
        //同名变量
        //&定义-直接修改地址
        vector<Var*>&list = *varTab[var->name];
        //寻找变量的作用域
        int i;
        for(i = 0; i < list.size(); i++) {
            if(list[i]->scopePath.size() == 0 || var->scopePath.size() == 0) continue;
            if(list[i]->scopePath.back() == var->scopePath.back()) {
                break;
            }
        }
        //常量
        if(i == list.size() || var->name[0] == '<') {
            list.push_back(var);
        } else {
            // 错误处理
            SEMERROR(VAR_RE_DEF, var->name);
            delete var;
            return;
        }
    }
    int flag = ir->genVarInit(var);
    if(curFun && flag) curFun->locate(var);
}
//添加字串常量-传引用
void SymTab::addStr(Var* &var) {
    unordered_map<string, Var*, string_hash>::iterator strIt, strEnd = strTab.end();
    for(strIt = strTab.begin(); strIt != strEnd; strIt++) {
        Var* str = strIt->second;
        if(var->strVal == str->strVal) {
            //字串常量已有此字串
            delete var;
            var = str;
            return;
        }
    }
    //字串表无此字串
    strTab[var->name] = var;
}
Var* SymTab::getVar(string name) {
    Var* select = NULL;
    //不管什么域，反正存在有此变量名的变量
    if(varTab.find(name) != varTab.end()) {
        //获取有此变量名的变量表
        //&定义-直接修改地址
        vector<Var*>&list = *varTab[name];
        //获取现在的scopePath长度
        int pathLen = scopePath.size();
        int maxLen = 0;
        //遍历同变量名的变量表，按照域匹配
        for(int i = 0; i < list.size(); i++) {
            //遍历到的变量的域长度
            int tmpLen = list[i]->scopePath.size();
            //遍历到的变量的域长度比现在的域小，且包含在现在的域
            if(tmpLen <= pathLen && list[i]->scopePath[tmpLen - 1] == scopePath[tmpLen - 1]) {
                //可能存在多个匹配，寻找最大的匹配
                if(tmpLen > maxLen) {
                    maxLen = tmpLen;
                    select = list[i];
                }
            }
        }
    }
    // 错误处理
    if(!select) 
        SEMERROR(VAR_UN_DEC, name);
    return select;
}
void SymTab::decFun(Fun* fun) {
    fun->isExtern = true;
    //前面没有声明过这个函数
    if(funTab.find(fun->name) == funTab.end()) {
        funTab[fun->name] = fun;
    } else {
        // 目前不支持重载
        Fun* last = funTab[fun->name];
        if(!last->match(fun)) {
            // 错误处理
            SEMERROR(FUN_DEC_ERR, fun->name);
        }
        delete fun;
    }
}
void SymTab::defFun(Fun* fun) {
    if(fun->isExtern) {
        // 错误处理
        SEMERROR(EXTERN_FUN_DEF, fun->name);
        fun->isExtern = false;
    }
    //前面没有声明过这个函数
    if(funTab.find(fun->name) == funTab.end()) {
        funTab[fun->name] = fun;
        // TODO IGNORE THIS
        // funList.push_back(fun->name);
    } else {
        Fun* last = funTab[fun->name];
        if(last->isExtern) {
            //匹配到的是函数声明
            if(!last->match(fun)) {
                // 错误处理
                SEMERROR(FUN_DEC_ERR, fun->name);
            }
            last->define(fun);
        } else {
            //重定义
            // 错误处理
            SEMERROR(FUN_RE_DEF, fun->name);
        }
        delete fun;
        fun = last;
    }
    curFun = fun;
    // 参数
    for(auto p = fun->paraVar.begin(); p != fun->paraVar.end(); ++p) {
        (*p)->scopePath = scopePath;
        addVar(*p);
    }
    ir->genFunHead(curFun);
}
void SymTab::endDefFun() {
    ir->genFunTail(curFun);
    curFun = NULL;
}
void SymTab::addInst(InterInst* inst) {
    if(curFun) curFun->interCode.push_back(inst);
    instList.push_back(inst);
}
vector<Var*> SymTab::getGlbVars() {
    vector<Var*> glbVars;
    unordered_map<string, vector<Var*>*, string_hash>::iterator varIt, varEnd = varTab.end();
    for(varIt=varTab.begin(); varIt != varEnd; ++varIt) {
        string varName = varIt->first;
        // 常量
        if(varName[0] == '<') continue;
        vector<Var*>& list = *varIt->second;
        for(int j = 0; j < list.size(); j++) {
            // global
            if(list[j]->scopePath.size() == 1) {
                glbVars.push_back(list[j]);
                break;
            }
        }
    }
    return glbVars;
}
//给变量设置类型 & 类型检查
void Var::setType(Tag t) {
    this->type = t;
    if(this->type == KW_VOID) {
        // 错误处理
        SEMERROR(VOID_VAR, "");
        this->type = KW_INT;
    }
    if(!this->isExtern && this->type == KW_INT) this->size = 4;
    else if(!this->isExtern && this->type == KW_CHAR) this->size = 1;
}
//给数组设置长度 & 类型检查
void Var::setArray(int len) {
    if(len <= 0) {
        // 错误处理
        SEMERROR(ARRAY_LEN_INVALID, name);
        return ;
    }
    isArray = true;
    isLeft = false;
    arraySize = len;
    if(!isExtern) size *= len;
}
Var* SymTab::one = new Var(new Num(1));
Var* SymTab::four = new Var(new Num(4));
Var* Var::getStep(Var* val) {
    // 基本类型（非指针）
    if(val->isBase()) return SymTab::one;
    // char* 类型
    else if(val->type == KW_CHAR) return SymTab::one;
    // int* 类型
    else if(val->type == KW_INT)return SymTab::four;
    else return NULL;
}
//数组的构造函数
Var::Var(vector<int> scopePath, bool isExtern, Tag tag, string id, int len) {
    clear();
    this->initData = NULL;
    this->isLeft = true;
    this->scopePath = scopePath;
    this->isExtern = isExtern;
    this->setType(tag);
    this->name = id;
    this->setArray(len);
    // cout<<"数组的构造函数"<<endl;
    // cout<<"> type: "<<type<<", name: "<<name<<", scopePath: ";
    // for(auto p3 = scopePath.begin(); p3 != scopePath.end(); ++p3) {
    //     cout<<"/"<<*p3;
    // }
    // cout<<endl;
}
//变量/指针变量的构造函数
Var::Var(vector<int> scopePath, bool isExtern, Tag tag, bool isPtr, string id, Var* initVal) {
    clear();
    this->isLeft = true;
    this->scopePath = scopePath;
    this->isExtern = isExtern;
    this->setType(tag);
    this->isPtr = isPtr;
    this->name = id;
    this->initData = initVal;
    // cout<<"变量/指针变量的构造函数"<<endl;
    // cout<<"> type: "<<type<<", name: "<<name<<", scopePath: ";
    // for(auto p3 = scopePath.begin(); p3 != scopePath.end(); ++p3) {
    //     cout<<"/"<<*p3;
    // }
    // cout<<endl;
}
//常量的构造函数
Var::Var(Token* it) {
    clear();
    this->initData = NULL;
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
            // GenCode
            this->name = GenIR::genTmpVar();
            this->strVal = ((Str*)it)->str;
            this->setArray(this->strVal.size() + 1);
            break;
        }
    }
    // cout<<"常量的构造函数"<<endl;
    // cout<<"> type: "<<type<<", name: "<<name<<", scopePath: ";
    // for(auto p3 = scopePath.begin(); p3 != scopePath.end(); ++p3) {
    //     cout<<"/"<<*p3;
    // }
    // cout<<endl;
}
// 中间变量，但不应该是常量！
Var::Var(vector<int> scopePath, Tag tag, bool isExtern) {
    clear();
    this->initData = NULL;
    switch(tag) {
        case KW_INT: {
            this->setType(KW_INT);
            break;
        }
        case KW_CHAR: {
            this->setType(KW_CHAR);
            this->intVal = 0;
            break;
        }
    }
    this->isLiteral = false;
    this->name = GenIR::genTmpVar();
    this->scopePath = scopePath;
    this->isExtern = isExtern;
    this->isLeft = false;
    // cout<<"中间变量，但不应该是常量！"<<endl;
    // cout<<"> type: "<<type<<", name: "<<name<<", scopePath: ";
    // for(auto p3 = scopePath.begin(); p3 != scopePath.end(); ++p3) {
    //     cout<<"/"<<*p3;
    // }
    // cout<<endl;
}

// 推测*2 产生一个中间变量，其类型同initVal
Var::Var(vector<int> scopePath, Var* initVal) {
    clear();
    this->scopePath = scopePath;
    this->isLiteral = initVal->isLiteral;
    this->type = initVal->type;
// DONE choice
    // GenIR 168行，生成了常量
    // 直接返回了，导致Return 常量
    // if(this->type == KW_INT)
    //     this->name = "<int>";
    // else
    //     this->name = "<char>";
    // 中间变量不应该是常量
    this->name = GenIR::genTmpVar();

    this->isPtr = initVal->isPtr;
    this->isArray = initVal->isArray;
    this->arraySize = initVal->arraySize;
    this->isLeft = initVal->isLeft;
    this->initData = initVal->initData;
    this->isInit = initVal->isInit;
    this->intVal = initVal->intVal;
    this->charVal = initVal->charVal;
    this->strVal = initVal->strVal;
    this->ptrVal = initVal->ptrVal;
    this->ptr = initVal->ptr;
    this->size = initVal->size;
    this->offset = initVal->offset;
    // cout<<"产生一个中间变量，其类型同initVal"<<endl;
    // cout<<"> type: "<<type<<", name: "<<name<<", scopePath: ";
    // for(auto p3 = scopePath.begin(); p3 != scopePath.end(); ++p3) {
    //     cout<<"/"<<*p3;
    // }
    // cout<<endl;
}
Var::Var() {
    clear();
    this->initData = NULL;
}
void Var::clear() {
    this->isLiteral = false;
    this->type = KW_INT;
    this->name = "NULL";

    this->isPtr = false;
    this->isArray = false;
    this->arraySize = false;
    this->isLeft = false;
    this->initData = NULL;
    this->isInit = false;
    this->intVal = 0;
    this->charVal = false;
    this->strVal = "";
    this->ptrVal = false;
    this->ptr = NULL;
    this->size = 0;
    this->offset = 0;
}
//返回值: 返回true，则需要先计算表达式的值后再对局部变量进行初始化，否则不需要。
bool Var::setInit() {
    Var* init = initData;
    if(!init) return false;
    isInit = false;
    if(isExtern) {
        // 错误处理
        SEMERROR(DEC_INIT_DENY, name);

        // 变量的初始化类型是否与变量的声明类型相同或可转换
    } else if (!GenIR::typeCheck(this, init)) {
        // 错误处理
        SEMERROR(VAR_INIT_ERR, name);
    } else if(init->isLiteral) {
        //常量
        isInit = true;
        if(init->isArray)
            ptrVal = init->name;
        else
            intVal = init->intVal;
    } else {
        //变量
        if(scopePath.size() == 1) {
            // 全局变量的初始化必须是常量
            // 错误处理
            SEMERROR(GLB_INIT_ERR, name);
        }
        //局部变量
        else return true;
    }
    return false;
}
bool Var::isVoid() {
    return this->type == KW_VOID;
}
bool Var::isBase() {
    return !(this->isPtr || this->isArray);
}
bool Var::isRef() {
    return (this->isPtr || this->isArray);
}
int Var::getVal() {
    if(this->type == KW_CHAR) return this->charVal;
    else return this->intVal;
}
// 猜测: 返回一个void变量
Var* Var::getVoid() {
    Var* var = new Var();
    var->type = KW_VOID;
    var->name = "<NULL>";
    return var;
}
Var* Var::getTrue() {
    Var* var = new Var();
    var->name = "<int>";
    var->type = KW_INT;
    var->isLiteral = true;
    var->intVal = 1;
    return var;
}
string Var::getRawStr() {
    // \0 \n \t => ascii(xx)
    // abc => "abc"
    // "\thello\nworld" => 9,"hello ",10,"world",0（逗号分隔ascii码，双引号分隔非特殊字符串）
    stringstream ss;
    int len = strVal.size();
    for(int i = 0, chPass = 0; i < len; i++) {
        if(strVal[i] == 10 || strVal[i] == 9 || strVal[i] == '\"' || strVal[i] == '\0') {
            if(chPass == 0) {
                if(i != 0) ss<<",";
                ss<<(int)strVal[i];
            } else
                ss<<"\","<<(int)strVal[i];
            chPass = 0;
        } else {
            if(chPass == 0) {
                if(i != 0) ss<<",";
                ss<<"\""<<strVal[i];
            } else 
                ss<<strVal[i];
            if(i == len - 1) ss<<"\"";
            chPass = 1;
        }
    }
    ss<<",0";
    return ss.str();
}
Fun::Fun(bool isExtern, Tag tag, string name, vector<Var*> paraList) {
    this->maxDepth = 0;
    this->curEsp = 0;
    this->isExtern = isExtern;
    this->type = tag;
    this->name = name;
    this->paraVar = paraList;
    this->scopeEsp.push_back(0);
}
Fun* SymTab::getFun(string name, vector<Var*>& args) {
    if(funTab.find(name) != funTab.end()) {
        Fun* last = funTab[name];
        if(!last->match(args)) {       
            // 错误处理
            SEMERROR(FUN_CALL_ERR, name);
            return NULL;
        }
        return last;
    }
    // 错误处理
    SEMERROR(FUN_UN_DEC, name);
    return NULL;
}
void Fun::enterScope() {
    //进入函数内的作用域
    //e.g:
    // int a() {
    //     while(1) {
    //          这里就是函数内的作用域内
    //     }
    // }
    //进入新域时，设置新域的大小为0
    scopeEsp.push_back(0);
}
void Fun::leaveScope() {
    //退出域时，计算最大深度(退出函数时用)
    maxDepth = (curEsp > maxDepth) ? curEsp : maxDepth;
    //目前的指针减少域大小(退回操作)
    curEsp -= scopeEsp.back();
    scopeEsp.pop_back();
}
void Fun::locate(Var* var) {
    int size = var->size;
    //将size补全至4的倍数
    size += (4 - size % 4) % 4;
    //作用域指针位置
    scopeEsp.back() += size;
    //当前栈指针位置
    curEsp += size;
    var->offset = -curEsp;
}
bool Fun::match(Fun* fun) {
    //参数名不同
    if(name != fun->name) return false;
    int len = paraVar.size();
    //参数长度不同
    if(len != fun->paraVar.size()) return false;
    for(int i = 0; i < len; i++) {
        //typeCheck:两种类型可以转换但是不同，例如int*与int[]
        if(GenIR::typeCheck(paraVar[i], fun->paraVar[i])) {
            if(paraVar[i]->type != fun->paraVar[i]->type) SEMWARN(FUN_DEC_CONFLICT, name);
        } else return false;
    }
    // 错误处理
    if(type != fun->type) SEMWARN(FUN_RET_CONFLICT, name);
    return true;
}
bool Fun::match(vector<Var*>& args) {
    int len = paraVar.size();
    //参数长度不同
    if(len != args.size()) return false;
    for(int i = 0; i < len; i++) {
        //typeCheck:两种类型可以转换但是不同，例如int*与int[]
        if(!GenIR::typeCheck(paraVar[i], args[i])) return false;
    }  
    return true;
}
void Fun::define(Fun* def) {
    isExtern = false;
    paraVar = def->paraVar;
}