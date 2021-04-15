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