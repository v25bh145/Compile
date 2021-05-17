#include "symTab.h"
using namespace std;
InterInst::InterInst() {
    cout<<"InterInst()"<<endl;
}
InterInst::InterInst(Operator op, Var* var) {
    cout<<"InterInst(Operator op, Var* var)"<<endl;
    this->op = op;
    this->arg1 = var;
}
InterInst::InterInst(Operator op, Fun* fun) {
    cout<<"InterInst(Operator op, Fun* fun) "<<fun->name<<endl;
    this->op = op;
    this->fun = fun;
}
InterInst::InterInst(Operator op, Fun* fun, Var* ret) {
    cout<<"InterInst(Operator op, Fun* fun, Var* ret)"<<endl;
    this->op = op;
    this->fun = fun;
    this->result = ret;
}
InterInst::InterInst(Operator op, InterInst* interInst, Var* var2, Var* var3){
    cout<<"InterInst(Operator op, InterInst* interInst, Var* var2, Var* var3))"<<endl;
    this->op = op;
    this->target = interInst;
    this->arg1 = var2;
    this->arg2 = var3;
}
InterInst::InterInst(Operator op, InterInst* interInst, Var* var){
    cout<<"InterInst(Operator op, InterInst* interInst, Var* var)"<<endl;
    this->op = op;
    this->target = interInst;
    this->arg1 = var;
}
InterInst::InterInst(Operator op, InterInst* interInst){
    cout<<"InterInst(Operator op, InterInst* interInst)"<<endl;
    this->op = op;
    this->target = interInst;
}
InterInst::InterInst(Operator op, Var* var1, Var* var2){
    cout<<"InterInst(Operator op, Var* var1, Var* var2)"<<endl;
    this->op = op;
    this->arg1 = var1;
    this->arg2 = var2;
}
// ???
InterInst::InterInst(Operator op, Var* var1, Var* var2, Var* var3){
    cout<<"InterInst(Operator op, Var* var1, Var* var2, Var* var3)"<<endl;
    this->op = op;
}