#include "generateX86.h"
GenerateX86::GenerateX86(string filePath) {
    this->file = fopen(filePath.c_str(), "w");
}
void GenerateX86::loadVar(string reg32, string reg8, Var* var) {
    if(!var) return;
    const char* reg = var->type == KW_CHAR ? reg8.c_str() : reg32.c_str();
    if(var->type == KW_CHAR) {
        // 将32位寄存器清零
        emit("mov %s, 0", reg32.c_str());
    }
    const char* name = var->name.c_str();
    if(!var->isLiteral) {
        int off = var->offset;
        if(!off) {
            if(!var->isArray)
                emit("mov %s, [%s]", reg, name);
            else
                emit("mov %s, %s", reg, name);
        } else {
            if(!var->isArray)
                emit("mov %s, [ebp%+d]", reg, off);
            else
                emit("lea %s, [ebp%+d]", reg, off);
        }
    } else {
        if(var->isBase())
            emit("mov %s, %d", reg, var->getVal());
        else
            emit("mov %s, %s", reg, name);
    }
}
void GenerateX86::leaVar(string reg32, Var* var) {
    if(!var) return;
    const char* reg = reg32.c_str();
    const char* name = var->name.c_str();

    int off = var->offset;
    if(!off)
        emit("mov %s, %s", reg, name);
    else
        emit("lea %s, [ebp%+d]", reg, off);
}
void GenerateX86::storeVar(string reg32, string reg8, Var* var) {
    if(!var) return;
    const char* reg = var->type == KW_CHAR ? reg8.c_str() : reg32.c_str();   
    const char* name = var->name.c_str();
    
    int off = var->offset;
    if(!off)
        emit("mov [%s], %s", name, reg);
    else
        emit("mov [ebp%+d], %s", off, reg);
}
void GenerateX86::initVar(Var* var) {
    if(!var) return;
    if(var->initData != NULL) {
        if(var->isBase()) 
            emit("mov eax, %s", var->ptrVal.c_str());
        else
            emit("mov eax, %s", var->ptrVal.c_str());
        storeVar("eax", "al", var);
    }
}
void GenerateX86::toX86(InterInst* interInst) {
    if(interInst->label != "") {
        fprintf(file, "%s: \n", interInst->label.c_str());
        return;
    }
    switch(interInst->op) {
        case OP_DEC:
            initVar(interInst->arg1);
            break;
        case OP_ENTRY:
            emit("push ebp");
            emit("mov ebp, esp");
            emit("sub esp, %d", interInst->fun->maxDepth);
            break;
        case OP_EXIT:
            emit("mov esp ebp");
            emit("pop ebp");
            emit("ret");
            break;
        case OP_AS:
            loadVar("eax", "al", interInst->arg1);
            storeVar("eax", "al", interInst->result);
            break;
        case OP_ADD:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("add eax, ebx");
            storeVar("eax", "al", interInst->result);
            break;
        case OP_SUB:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("sub eax, ebx");
            storeVar("eax", "al", interInst->result);
            break;
        case OP_MUL:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("imul eax, ebx");
            storeVar("eax", "al", interInst->result);
            break;
        case OP_DIV:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("idiv eax, ebx");
            storeVar("eax", "al", interInst->result);
            break;
        case OP_NEG:
            loadVar("eax", "al", interInst->arg1);
            emit("neg eax");
            storeVar("eax", "al", interInst->result);
            break;
        case OP_GT:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("mov ecx, 0");
            emit("cmp eax, ebx");
            emit("setg cl");
            storeVar("ecx", "cl", interInst->result);
            break;
        case OP_GE:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("mov ecx, 0");
            emit("cmp eax, ebx");
            emit("setge cl");
            storeVar("ecx", "cl", interInst->result);
            break;
        case OP_LT:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("mov ecx, 0");
            emit("cmp eax, ebx");
            emit("setl cl");
            storeVar("ecx", "cl", interInst->result);
            break;
        case OP_LE:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("mov ecx, 0");
            emit("cmp eax, ebx");
            emit("setle cl");
            storeVar("ecx", "cl", interInst->result);
            break;
        case OP_EQU:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("mov ecx, 0");
            emit("cmp eax, ebx");
            emit("sete cl");
            storeVar("ecx", "cl", interInst->result);
            break;
        case OP_NE:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("mov ecx, 0");
            emit("cmp eax, ebx");
            emit("setne cl");
            storeVar("ecx", "cl", interInst->result);
            break;
        // 逻辑非
        case OP_NOT:
            loadVar("eax", "al", interInst->arg1);
            emit("mov ebx, 0");
            emit("cmp eax, 0");
            emit("sete bl");
            storeVar("ebx", "bl", interInst->result);
            break;
        // 逻辑与
        case OP_AND:
            loadVar("eax", "al", interInst->arg1);
            emit("cmp eax, 0");
            emit("setne cl");
            loadVar("ebx", "bl", interInst->arg2);
            emit("cmp ebx, 0");
            emit("setne bl");
            emit("and eax, abx");
            storeVar("eax", "al", interInst->result);
            break;
        // 逻辑或
        case OP_OR:
            loadVar("eax", "al", interInst->arg1);
            emit("cmp eax, 0");
            emit("setne al");
            loadVar("ebx", "bl", interInst->arg2);
            emit("cmp ebx, 0");
            emit("setne bl");
            emit("or eax, abx");
            storeVar("eax", "al", interInst->result);
            break;
        case OP_JMP:
            emit("jmp %s", interInst->target->label.c_str());
            break;
        case OP_JT:
            loadVar("eax", "al", interInst->arg1);
            emit("cmp eax, 0");
            emit("jne %s", interInst->target->label.c_str());
            break;
        case OP_JF:
            loadVar("eax", "al", interInst->arg1);
            emit("cmp eax, 0");
            emit("je %s", interInst->target->label.c_str());
            break;
        case OP_JNE:
            loadVar("eax", "al", interInst->arg1);
            loadVar("ebx", "bl", interInst->arg2);
            emit("cmp eax, ebx");
            emit("jne %s", interInst->target->label.c_str());
            break;
        case OP_ARG:
            loadVar("eax", "al", interInst->arg1);
            emit("push eax");
            break;
        case OP_PROC:
            emit("call %s", interInst->fun->name.c_str());
            emit("add esp, %d", interInst->fun->paraVar.size() * 4);
            break;
        case OP_CALL:
            emit("call %s", interInst->fun->name.c_str());
            emit("add esp, %d", interInst->fun->paraVar.size() * 4);
            storeVar("eax", "al", interInst->result);
            break;
        case OP_RET:
            emit("jmp %s", interInst->target->label.c_str());
            break;
        case OP_RETV:
            loadVar("eax", "al", interInst->arg1);
            emit("jmp %s", interInst->target->label.c_str());
            break;
        case OP_LEA:
            leaVar("eax", interInst->arg1);
            storeVar("eax", "al", interInst->result);
            break;
        case OP_SET:
            loadVar("eax", "al", interInst->result);
            loadVar("ebx", "bl", interInst->arg1);
            emit("mov [ebx], eax");
            break;
        case OP_GET:
            loadVar("eax", "al", interInst->arg1);
            emit("mov eax, [eax]");
            storeVar("eax", "al", interInst->result);
            break;
    }
}
void GenerateX86::genData(SymTab* symTab) {
    vector<Var*> glbVars = symTab->getGlbVars();
    for(unsigned int i = 0; i < glbVars.size(); i++) {
        Var* var = glbVars[i];
        fprintf(file, "global %s\n", var->name.c_str());
        fprintf(file, "\t%s ", var->name.c_str());
        int typeSize = var->type == KW_CHAR ? 1 : 4;
        if(var->isArray) 
            fprintf(file, "times %d ", var->size / typeSize);
            // 是char且不是char* => db
        const char* type = (var->type == KW_CHAR && !var->isPtr) ? "db" : "dd";
        fprintf(file, "%s ", type);
        if(var->isInit) {
            if(var->isBase())
                fprintf(file, "%d\n", var->getVal());
            else
                fprintf(file, "%s\n", var->ptrVal.c_str());
        } else
            fprintf(file, "0\n");
    }
    for(auto strIt = symTab->strTab.begin(); strIt != symTab->strTab.end(); ++strIt) {
        Var* str = strIt->second;
        fprintf(file, "\t%s db %s\n", str->name.c_str(), str->getRawStr().c_str());
    }
}
void GenerateX86::genAsm(SymTab* symTab) {
    fprintf(file, "section .data\n");
    genData(symTab);
    fprintf(file, "section .text\n");
    for(auto funIt = symTab->funTab.begin(); funIt != symTab->funTab.end(); ++funIt) {
        Fun* fun = funIt->second;
        fprintf(file, "global %s\n", fun->name.c_str());
        fprintf(file, "%s:\n", fun->name.c_str());
        vector<InterInst*>& code = fun->interCode;
        cout<<"function's interInst size: "<<code.size()<<endl;
        for(auto instIt = code.begin(); instIt != code.end(); ++instIt) 
            toX86(*instIt);
    }
}