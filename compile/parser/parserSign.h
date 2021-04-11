/**
 * 语义分析产生的对象节点
 **/
#ifndef PARSERSIGN_H
#define PARSERSIGN_H
#include "../lexical/lexical.h"
using namespace std;
void initNonterminalMap();
string getNonterminalMap(int i);
enum NonterminalType{
    PROGRAM, SEGMENT, 
    TYPE, DEF, 
    DEFDATA, VARRDEF, 
    INIT, EXPR, 
    DEFLIST, FUN, 
    PARA, BLOCK, 
    FUNTAIL, IDTAIL, 
    PARADATA, PARADATATAIL, 
    PARALIST, SUBPROGRAM, 
    LOCALDEF, STATEMENT, 
    ASSEXPR, ASSTAIL,
    OREXPR, ORTAIL,
    ANDEXPR, ANDTAIL,
    CMPEXPR, CMPTAIL, CMPS,
    ALOEXPR, ALOTAIL, 
    ADDS, MULS, 
    ITEM, FACTOR, ITEMTAIL, 
    LOP, VAL, ELEM, ROP, 
    IDEXPR, LITERAL, REALARG, 
    ARGLIST, ARG, ALTEXPR, 
    WHILESTAT, FORSTAT, DOWHILESTAT, 
    IFSTAT, SWITCHSTAT, 
    FORINIT, ELSESTAT, CASESTAT, 
    CASELABEL, ROOT
};
class ParserSign {
    public:
        bool isTerminal;
        string toString();
};
class Nonterminal: public ParserSign {
    private:
        NonterminalType type;
        //fifo
        list<ParserSign*> children;
    public:
        Nonterminal(NonterminalType type) {
            this->type = type;
            this->isTerminal = false;
        }
        void setType(NonterminalType type) {
            this->type = type;
        }
        NonterminalType getType() {
            return this->type;
        }
        void setChild(ParserSign* child) {
            this->children.push_back(child);
        }
        list<ParserSign*> getChildren() {
            return this->children;
        }
        string toString() {
            return getNonterminalMap(type);
        }
};
class Terminal: public ParserSign {
    private:
        Tag type;
    public:
        Terminal(Tag type) {
            this->type = type;
            this->isTerminal = true;
        }
        void setType(Tag type) {
            this->type = type;
        }
        Tag getType() {
            return this->type;
        }
        string toString() {
            return getTagMap(type);
        }
};

#endif