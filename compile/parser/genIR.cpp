#include "./symTab.h"
using namespace std;
bool GenIR::genVarInit(Var* var) {
    //处理整型、字符型常量的Var对象，他们不需要初始化
    if(var->name[0] == '<') return false;
    //TODO ???
    // symtab.addInss(new InterInst(OP_DEC, var));
    //TODO genTwoOp
    // 返回true，说明需要生成赋值代码
    // if(var->setInit()) 
    //     genTwoOp(var, ASSIGN, var->getInitData());
    return true;
}
// 处理单目后缀运算符表达式
Var* GenIR::genOneOpRight(Var* val, Tag opt) {
    if(!val) return NULL;
    //TODO Var::isVoid
    // if(val->isVoid()) {
        // TODO 错误处理
    //     SEMERROR(EXPR_IS_VOID);
    //     return NULL;
    // }
    //不是左值表达式不能接单目后缀运算符
    //不可访问存储区域的表达式不可以被++、--、&操作
    if(!val->isLeft) {
        // TODO 错误处理
        // SEMERROR(EXPR_NOT_LEFT_VAL);
        return val;
    }
    // TODO genIncR genDecR
    // 右自增
    // if(opt == INC) return genIncR(val);
    // 右自减
    // if(opt == DEC) return genDecR(val);
    return val;
}
Var* GenIR::genArray(Var* array, Var* index) {
    if(!array || !index) return NULL;
    //TODO Var::isVoid
    // if(array->isVoid() || index->isVoid()) {
        // TODO 错误处理
    //     SEMERROR(EXPR_IS_VOID);
    //     return NULL;
    // }
    // 数组变量对象不能是基本类型(int、char，必须是int*、char*、int[]等等)
    //TODO Var::isBase
    // if(array->isBase() ||!index->isBase()) {
        // TODO 错误处理
    //     SEMERROR(ARR_TYPE_ERR);
    //     return index;
    // }
    // TODO genPtr genAdd
    // return genPtr(genAdd(array, index));
}
void GenIR::genReturn(Var* ret) {
    if(!ret) return;
    //TODO ???
    // Fun* fun = symtab.getCurFun();
    //TODO Var::isVoid isBase
    // 返回值不是void，则不能返回void; 返回值是void，则只能返回void
    // if(ret->isVoid() && fun->getType() != KW_VOID || ret->isBase() && fun->getType() == KW_VOID) {
        // TODO 错误处理
    //     SEMERROR(RETURN_ERR);
    //     return;
    // } 
    //TODO InterInst isVoid getReturnPoint symtab
    // InterInst* returnPoint = fun->getReturnPoint();
    // if(ret->isVoid())
    //     symtab.addInst(new InterInst(OP_RET, returnPoint));
    // else {
    //     if(ret->isRef())
    //         ret = genAssign(ret);
    //     symtab.addInst(new InterInst(OP_RETV, returnPoint, ret));
    // } 
}