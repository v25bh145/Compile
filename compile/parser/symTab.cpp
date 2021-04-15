#include "symTab.h"
using namespace std;
void SymTab::enter() {
    scopeId++;
    scopePath.push_back(scopeId);
    //如果当前在函数作用域内
    if(curFun) curFun->enterScope();
}