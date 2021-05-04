#include "symTab.h"
#include "semError.h"
using namespace std;
GenIR::GenIR(SymTab* symTab) {
    this->symTab = symTab;
}
bool GenIR::genVarInit(Var* var) {
    //处理整型、字符型常量的Var对象，他们不需要初始化
    if(var->name[0] == '<') return false;
    symTab->addInst(new InterInst(OP_DEC, var));
    // 返回true，说明需要生成赋值代码
    // TODO getInitData
    // if(var->setInit()) 
        // genTwoOp(var, ASSIGN, var->getInitData());
    return true;
}
void GenIR::genReturn(Var* ret) {
    if(!ret) return;
    Fun* fun = symTab->curFun;
    // 返回值不是void，则不能返回void; 返回值是void，则只能返回void
    if(ret->isVoid() && fun->type != KW_VOID || ret->isBase() && fun->type == KW_VOID) {
        // 错误处理
        SEMERROR(RETURN_ERR, fun->name);
        return;
    } 
    InterInst* returnPoint = fun->getReturnPoint();
    if(ret->isVoid()) {
        symTab->addInst(new InterInst(OP_RET, returnPoint));
    }
    else {
        //isRef ret是*p情况
        if(ret->isRef()) {
            ret = genAssign(ret);
        }
        symTab->addInst(new InterInst(OP_RETV, returnPoint, ret));
    } 
}
void GenIR::genFunHead(Fun* function) {
    function->enterScope();
    symTab->addInst(new InterInst(OP_ENTRY, function));
    function->returnPoint = new InterInst;
}
void GenIR::genFunTail(Fun* function) {
    symTab->addInst(function->returnPoint);
    symTab->addInst(new InterInst(OP_EXIT, function));
    function->leaveScope();
}
Var* GenIR::genPtr(Var* val) {
    if(val->isBase()) {
        SEMERROR(EXPR_IS_BASE, val->name);
        return val;
    }
    // Var construct
    Var* tmp = new Var(symTab->scopePath, val->type, false);
    tmp->isLeft = true;
    // TODO Var setPointer
    // tmp.setPointer(val);
    symTab->addVar(tmp);
    return tmp;
}  
Var* GenIR::genLea(Var* val) {
    // TODO Var getLeft
    // if(!val->getLeft()) {
        // SEMERROR(EXPR_NOT_LEFT_VAL, val->name);
        // return val;
    // }
    if(val->isRef())
        return val->ptr;
    else {
        // Var construct
        Var* tmp;
        tmp = new Var(symTab->scopePath, val->type, true);
        symTab->addVar(tmp);
        symTab->addInst(new InterInst(OP_LEA, tmp, val));
    }
}
//将指针运算结果作为右值
Var* GenIR::genAssign(Var* val) {
    Var* tmp;
    // Var construct
    tmp = new Var(symTab->scopePath, val);
    symTab->addVar(tmp);
    if(val->isRef())
        symTab->addInst(new InterInst(OP_GET, tmp, val->ptr));
    else
        symTab->addInst(new InterInst(OP_AS, tmp, val));
    return tmp;
}
//将指针运算结果作为左值
Var* GenIR::genAssign(Var* lval, Var* rval) {
    // TODO Var getLeft
    // if(!lval->getLeft()) {
        // SEMERROR(EXPR_NOT_LEFT_VAL, lval->name);
        // return lval;
    // }
    // typeCheck
    if(!typeCheck(lval, rval)) {
        SEMERROR(ASSIGN_TYPE_ERR, rval->name);
        return rval;
    }
    if(rval->isRef())
        rval = genAssign(rval);
    if(lval->isRef())
        symTab->addInst(new InterInst(OP_SET, rval, lval->ptr));
    else
        symTab->addInst(new InterInst(OP_AS, lval, rval));
    return lval;
}
bool GenIR::typeCheck(Var* lval, Var* rval) {
    bool flag = false;
    if(!rval) return false;
    if(lval->isBase() && rval->isBase()) flag = true;
    else if(!lval->isBase() && !rval->isBase()) flag = (lval->type == rval->type);
    return flag;
}
Var* GenIR::genTwoOp(Var* lval, Tag opt, Var* rval) {
    if(!lval || !rval) return NULL;

    if(lval->isVoid() || rval->isVoid()) {
        SEMERROR(EXPR_IS_VOID, rval->name);
        return NULL;
    }

    if(opt == ASSIGN) return genAssign(lval, rval);

    if(lval->isRef()) lval = genAssign(lval);
    if(rval->isRef()) rval = genAssign(rval);

    if(opt == OR) return genOr(lval, rval);
    if(opt == AND) return genAnd(lval, rval);
    if(opt == EQU) return genEqu(lval, rval);
    if(opt == NEQU) return genNequ(lval, rval);
    if(opt == ADD) return genAdd(lval, rval);
    if(opt == SUB) return genSub(lval, rval);

    if(!lval->isBase() || !rval->isBase()) {
        SEMERROR(EXPR_NOT_BASE, rval->name);
        return lval;
    }

    if(opt == GT) return genGt(lval, rval);
    if(opt == GE) return genGe(lval, rval);
    if(opt == LT) return genLt(lval, rval);
    if(opt == LE) return genLe(lval, rval);
    if(opt == MUL) return genMul(lval, rval);
    if(opt == DIV) return genDiv(lval, rval);
    if(opt == MOD) return genMod(lval, rval);
}
/* 双目运算符 */
Var* GenIR::genAdd(Var* lval, Var* rval) {
    Var* tmp = NULL;
    if(!lval->isBase() && rval->isBase()) {
        tmp = new Var(symTab->scopePath, lval);
        // 将rval修正为实际的累加值
        rval = genMul(rval, Var::getStep(lval));
    } else if(lval->isBase() && rval->isBase()) {
        tmp = new Var(symTab->scopePath, KW_INT, false);
    } else {
        SEMERROR(EXPR_NOT_BASE, rval->name);
        return lval;
    }
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_ADD, tmp, lval, rval));
}
Var* GenIR::genSub(Var* lval, Var* rval) {
    Var* tmp = NULL;
    if(!rval->isBase()) {
        //1 - p is not allowed
        SEMERROR(EXPR_NOT_BASE, rval->name);
    }
    if(!lval->isBase()) {
        tmp = new Var(symTab->scopePath, lval);
        // 将rval修正为实际的累加值
        rval = genMul(rval, Var::getStep(lval));
    } else {
        tmp = new Var(symTab->scopePath, KW_INT, false);
    }
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_SUB, tmp, lval, rval));
}
Var* GenIR::genMul(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_MUL, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genOr(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_OR, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genAnd(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_AND, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genEqu(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_EQU, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genNequ(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_NE, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genGt(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_GT, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genGe(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_GE, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genLt(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_LT, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genLe(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_LE, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genDiv(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_DIV, tmp, lval, rval));
    return tmp;
}
Var* GenIR::genMod(Var* lval, Var* rval) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_MOD, tmp, lval, rval));
    return tmp;
}

/* 处理单目前缀运算符表达式 */
Var* GenIR::genOneOpLeft(Tag opt, Var* val) {
    if(!val) return NULL;
    if(val->isVoid()) {
        SEMERROR(EXPR_IS_VOID, val->name);
        return NULL;
    }
    if(opt == LEA) return genLea(val); // 取址
    else if(opt == MUL) return genPtr(val); //指针
    else if(opt == INC) return genIncL(val); //自加
    else if(opt == DEC) return genDecL(val); //自减
    else if(val->isRef()) return genAssign(val); //处理*p
    else if(opt == NOT) return genNot(val); //非
    else if(opt == SUB) return genMinus(val); //取负
    return val;
}
Var* GenIR::genIncL(Var* val) {
    if(!val->isLeft) {
        SEMERROR(EXPR_NOT_LEFT_VAL, val->name);
        return val;
    }
    //++*p
    if(val->isRef()) {
        Var* t1 = genAssign(val);
        Var* t2 = genAdd(t1, Var::getStep(val));
        genAssign(val, t2);
    } else symTab->addInst(new InterInst(OP_ADD, val, val, Var::getStep(val)));
    return val;
}
Var* GenIR::genDecL(Var* val) {
    if(!val->isLeft) {
        SEMERROR(EXPR_NOT_LEFT_VAL, val->name);
        return val;
    }
    //++*p
    if(val->isRef()) {
        Var* t1 = genAssign(val);
        Var* t2 = genSub(t1, Var::getStep(val));
        genAssign(val, t2);
    } else symTab->addInst(new InterInst(OP_SUB, val, val, Var::getStep(val)));
    return val;
}
Var* GenIR::genMinus(Var* val) {
    if(!val->isBase()) {
        SEMERROR(EXPR_NOT_BASE, val->name);
        return val;
    }
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_NEG, tmp, val));
}
Var* GenIR::genNot(Var* val) {
    Var* tmp = new Var(symTab->scopePath, KW_INT, false);
    symTab->addVar(tmp);
    symTab->addInst(new InterInst(OP_NOT, tmp, val));
}

/* 处理单目后缀运算符表达式 */
Var* GenIR::genOneOpRight(Var* val, Tag opt) {
    if(!val) return NULL;
    if(val->isVoid()) {
        // 错误处理
        SEMERROR(EXPR_IS_VOID, val->name);
        return NULL;
    }
    //不是左值表达式不能接单目后缀运算符
    //不可访问存储区域的表达式不可以被++、--、&操作
    if(!val->isLeft) {
        // 错误处理
        SEMERROR(EXPR_NOT_LEFT_VAL, val->name);
        return val;
    }
    // 右自增
    if(opt == INC) return genIncR(val);
    // 右自减
    if(opt == DEC) return genDecR(val);
    //先进行自加运算，再返回操作数结果
    return val;
}
Var* GenIR::genIncR(Var* val) {
    Var* tmp = genAssign(val);
    // (*p)++
    if(val->isRef()) {
        Var* t2 = genAdd(tmp, Var::getStep(val));
        genAssign(val, t2);
    } else symTab->addInst(new InterInst(OP_ADD, val, val, Var::getStep(val)));
    //先返回操作数结果，再进行自加运算
    return tmp;
}
Var* GenIR::genDecR(Var* val) {
    Var* tmp = genAssign(val);
    // (*p)++
    if(val->isRef()) {
        Var* t2 = genSub(tmp, Var::getStep(val));
        genAssign(val, t2);
    } else symTab->addInst(new InterInst(OP_SUB, val, val, Var::getStep(val)));
    //先返回操作数结果，再进行自加运算
    return tmp;
}

/* 数组索引运算表达式 */
Var* GenIR::genArray(Var* array, Var* index) {
    if(!array || !index) return NULL;
    if(array->isVoid() || index->isVoid()) {
        SEMERROR(EXPR_IS_VOID, array->name + ", " + index->name);
        return NULL;
    }
    // 数组变量对象不能是基本类型(int、char，必须是int*、char*、int[]等等)
    if(array->isBase() || !index->isBase()) {
        SEMERROR(ARR_TYPE_ERR, array->name + ", " + index->name);
        return index;
    }
    //a[i] => *(a + i)
    return genPtr(genAdd(array, index));
}

/* 函数调用表达式 */
void GenIR::genPara(Var* arg) {
    if(arg->isRef()) arg = genAssign(arg);
    //压栈
    InterInst* argInst = new InterInst(OP_ARG, arg);
    symTab->addInst(argInst);
}
Var* GenIR::genCall(Fun* function, vector<Var*>& args) {
    if(!function) return NULL;
    //入栈
    for(int i = args.size() - 1; i >= 0; i--) {
        genPara(args[i]);
    }
    //生成无返回的调用 => proc
    if(function->type == KW_VOID) {
        symTab->addInst(new InterInst(OP_PROC, function));
        return Var::getVoid();
    //生成有返回的调用 => call
    } else {
        Var* ret = new Var(symTab->scopePath, function->type, false);
        symTab->addInst(new InterInst(OP_CALL, function, ret));
        symTab->addVar(ret);
        return ret;
    }
}