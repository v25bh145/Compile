#ifndef GENERATEX86_H
#define GEBERATEX86_H
#include <iostream>
#include "../parser/parser.h"
#define emit(fmt, args...) fprintf(file, "\t" fmt "\n", ##args)

class GenerateX86 {
private:
    FILE* file;
    // 具体的生成
    void loadVar(string reg32, string reg8, Var* var);
    void leaVar(string reg32, Var* var);
    void storeVar(string reg32, string reg8, Var* var);
    void initVar(Var* var);
    void toX86(InterInst* interInst);
    void genData(SymTab* symTab);
public:
    // api
    GenerateX86(string filePath);
    void genAsm(SymTab* symTab);
};
#endif