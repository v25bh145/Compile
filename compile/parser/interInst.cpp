#include "symTab.h"
using namespace std;
int a;
InterInst::InterInst() {
    this->label = GenIR::genLb();
    // cout<<"InterInst()"<<endl;
    this->op = OP_NOP;
    this->arg1 = NULL;
    this->arg2 = NULL;
    this->fun = NULL;
    this->result = NULL;
    this->target = NULL;
    //print();
}
InterInst::InterInst(Operator op, Var* var) {
    this->label = "";
    // cout<<"InterInst(Operator op, Var* var)"<<endl;
    this->op = op;
    this->arg1 = var;

    this->arg2 = NULL;
    this->fun = NULL;
    this->result = NULL;
    this->target = NULL;
    //print();
}
InterInst::InterInst(Operator op, Fun* fun) {
    this->label = "";
    // cout<<"InterInst(Operator op, Fun* fun) "<<fun->name<<endl;
    this->op = op;
    this->fun = fun;

    this->arg2 = NULL;
    this->arg1 = NULL;
    this->result = NULL;
    this->target = NULL;
    //print();
}
InterInst::InterInst(Operator op, Fun* fun, Var* ret) {
    this->label = "";
    // cout<<"InterInst(Operator op, Fun* fun, Var* ret)"<<endl;
    this->op = op;
    this->fun = fun;
    this->result = ret;

    this->arg2 = NULL;
    this->arg1 = NULL;
    this->target = NULL;
    //print();
}
InterInst::InterInst(Operator op, InterInst* interInst, Var* var2, Var* var3){
    this->label = "";
    // cout<<"InterInst(Operator op, InterInst* interInst, Var* var2, Var* var3))"<<endl;
    this->op = op;
    this->target = interInst;
    this->arg1 = var2;
    this->arg2 = var3;

    this->fun = NULL;
    this->result = NULL;
    //print();
}
InterInst::InterInst(Operator op, InterInst* interInst, Var* var){
    this->label = "";
    // cout<<"InterInst(Operator op, InterInst* interInst, Var* var)"<<endl;
    this->op = op;
    this->target = interInst;
    this->arg1 = var;

    this->arg2 = NULL;
    this->result = NULL;
    this->fun = NULL;
    //print();
}
InterInst::InterInst(Operator op, InterInst* interInst){
    this->label = "";
    // cout<<"InterInst(Operator op, InterInst* interInst)"<<endl;
    this->op = op;
    this->target = interInst;

    this->arg1 = NULL;
    this->arg2 = NULL;
    this->result = NULL;
    this->fun = NULL;
    //print();
}
InterInst::InterInst(Operator op, Var* var1, Var* var2){
    this->label = "";
    // cout<<"InterInst(Operator op, Var* var1, Var* var2)"<<endl;
    this->op = op;
    this->arg1 = var1;
    this->arg2 = var2;

    this->target = NULL;
    this->result = NULL;
    this->fun = NULL;
    //print();
}
InterInst::InterInst(Operator op, Var* var1, Var* var2, Var* var3){
    this->label = "";
    // cout<<"InterInst(Operator op, Var* var1, Var* var2, Var* var3)"<<endl;
    this->op = op;

    this->arg1 = var2;
    this->arg2 = var3;
    this->target = NULL;
    this->result = var1;
    this->fun = NULL;
    //print();
}
void InterInst::print() {
    cout<<"Inst:"<<endl;
    if((this)->arg1 != NULL) {
        Var* tmp = (this)->arg1;
        
        cout<<"     arg1 name: "<<tmp->name<<", tag: "<<tmp->type<<", scopePath: ";
        
        for(auto p2 = tmp->scopePath.begin(); p2 != tmp->scopePath.end(); ++p2) {
            cout<<"/"<<*p2;
        }
        
        cout<<endl;
    }
    
    if((this)->arg2 != NULL) {
        Var* tmp = (this)->arg2;
        cout<<"     arg2 name: "<<tmp->name<<", tag: "<<tmp->type<<", scopePath: ";
        for(auto p2 = tmp->scopePath.begin(); p2 != tmp->scopePath.end(); ++p2) {
            cout<<"/"<<*p2;
        }
        cout<<endl;
    }
    if((this)->result != NULL) {
        Var* tmp = (this)->result;
        cout<<"     result name: "<<tmp->name<<", tag: "<<tmp->type<<", scopePath: ";
        for(auto p2 = tmp->scopePath.begin(); p2 != tmp->scopePath.end(); ++p2) {
            cout<<"/"<<*p2;
        }
        cout<<endl;
    }
    
    if((this)->fun != NULL) {
        Fun* tmp = (this)->fun;
        cout<<"     fun name: "<<tmp->name<<", tag: "<<tmp->type<<", scopeEsp: ";
        for(auto p2 = tmp->scopeEsp.begin(); p2 != tmp->scopeEsp.end(); ++p2) {
            cout<<"/"<<*p2;
        }
        cout<<endl;
    }
    
    cout<<"     op: "<<(this)->op<<endl;
    
    cout<<"     label: "<<(this)->label<<endl;
    
    if((this)->target != NULL) {
        cout<<"     has target"<<endl;
    }
}