/**
 * in book
 * */
// DONE 安装模块 5-24 0:00
// 1. main的esp减法奇奇怪怪 DONE
// 2. 常量返回值 DONE
// 3. 有NULL变量 DONE
// 4. 出现莫名neg指令 DONE
// FIXME: debug
#include <iostream>
#include <list>
#include "compile.h"
#include "lexical/lexical.h"
#include "lexical/scanner/scanner.h"
#include "parser/parserSign.h"
#include "parser/parser.h"
#include "generateX86/generateX86.h"
using namespace std;

int Compile(char* filePath, char* outPath) {

    cout<<"======词法分析======"<<endl;
    list<Token*> tokenList;
    if(readFile(filePath) < 0) return -1;
    tokenList = lexical_analyze();
    for(auto p = tokenList.begin(); p != tokenList.end(); p++) {
        cout<<(*p)->toString()<<endl;
    }

    cout<<"======词法分析======"<<endl;

    Parser* parser = new Parser(tokenList);
    Nonterminal *root = parser->run();

    // cout<<"======词法树分析======"<<endl;
    // parser->print(root, 0);

    cout<<"======符号表分析======"<<endl;
    SymTab* symTab = parser->getSymTab();

    cout<<"变量"<<endl;
    auto varTab = symTab->varTab;
    for(auto p = varTab.begin(); p != varTab.end(); ++p) {
        for(auto p2 = (*p).second->begin(); p2 != (*p).second->end(); ++p2) {
            cout<<"> type: "<<(*p2)->type<<", name: "<<(*p2)->name<<", scopePath: ";
            for(auto p3 = (*p2)->scopePath.begin(); p3 != (*p2)->scopePath.end(); ++p3) {
                cout<<"/"<<*p3;
            }
            cout<<endl;
        }
    }

    cout<<"函数"<<endl;
    auto funTab = symTab->funTab;
    for(auto p = funTab.begin(); p != funTab.end(); ++p) {
            cout<<"> type: "<<(*p).second->type<<", name: "<<(*p).second->name<<", scopeEsp: ";
            for(auto p2 = (*p).second->scopeEsp.begin(); p2 != (*p).second->scopeEsp.end(); ++p2) {
                cout<<"/"<<*p2;
            }
            cout<<endl<<"varList:"<<endl;
            for(auto p2 = (*p).second->paraVar.begin(); p2 != (*p).second->paraVar.end(); ++p2) {
                cout<<"     > type: "<<(*p2)->type<<", name: "<<(*p2)->name<<", scopePath: ";
                for(auto p3 = (*p2)->scopePath.begin(); p3 != (*p2)->scopePath.end(); ++p3) {
                    cout<<"/"<<*p3;
                }
                cout<<endl;
            }
            cout<<endl;
    }
    cout<<"======符号表分析======"<<endl;

    cout<<"======中间代码分析======"<<endl;
    vector<InterInst*> instList = symTab->instList;
    for(auto p = instList.begin(); p != instList.end(); ++p) {
        cout<<"Inst:"<<endl;
        if((*p)->arg1 != NULL) {
            Var* tmp = (*p)->arg1;
            
            cout<<"     arg1 name: "<<tmp->name<<", tag: "<<tmp->type<<", scopePath: ";
            
            for(auto p2 = tmp->scopePath.begin(); p2 != tmp->scopePath.end(); ++p2) {
                cout<<"/"<<*p2;
            }
            
            cout<<endl;
        }
        
        if((*p)->arg2 != NULL) {
            Var* tmp = (*p)->arg2;
            cout<<"     arg2 name: "<<tmp->name<<", tag: "<<tmp->type<<", scopePath: ";
            for(auto p2 = tmp->scopePath.begin(); p2 != tmp->scopePath.end(); ++p2) {
                cout<<"/"<<*p2;
            }
            cout<<endl;
        }
        if((*p)->result != NULL) {
            Var* tmp = (*p)->result;
            cout<<"     result name: "<<tmp->name<<", tag: "<<tmp->type<<", scopePath: ";
            for(auto p2 = tmp->scopePath.begin(); p2 != tmp->scopePath.end(); ++p2) {
                cout<<"/"<<*p2;
            }
            cout<<endl;
        }
        
        if((*p)->fun != NULL) {
            Fun* tmp = (*p)->fun;
            cout<<"     fun name: "<<tmp->name<<", tag: "<<tmp->type<<", scopeEsp: ";
            for(auto p2 = tmp->scopeEsp.begin(); p2 != tmp->scopeEsp.end(); ++p2) {
                cout<<"/"<<*p2;
            }
            cout<<endl;
        }
        
        cout<<"     op: "<<(*p)->op<<endl;
        
        cout<<"     label: "<<(*p)->label<<endl;
        
        if((*p)->target != NULL) {
            cout<<"     has target"<<endl;
        }
    }
    cout<<"======中间代码分析======"<<endl;

    cout<<"======生成asm分析======"<<endl;
    GenerateX86* generator = new GenerateX86(outPath);
    generator->genAsm(symTab);
    cout<<"======生成asm分析======"<<endl;
}