#include "symTab.h"
using namespace std;
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
            if(list[i]->scopePath.back() == var->scopePath.back()) {
                break;
            }
        }
        //常量
        if(i == list.size() || var->name[0] == '<') {
            list.push_back(var);
        } else {
            // TODO 变量重定义
            // SEMERROR(VAR_RE_DEF, var->name);
            delete var;
            return;
        }
        //TODO GenCode
        // if(ir) {
        //     int flag = ir->genVarInit(var);
        //     if(curFun && flag) curFun->locate(var);
        // }

    }
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
    //TODO 错误处理
    // if(!select) 
        // SEMERROR(VAR_UN_DEC, name);
    return select;
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