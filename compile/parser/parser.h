#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <list>
#include <vector>
#include "parserSign.h"
#include "../lexical/lexical.h"
#include "./symTab.h"
using namespace std;

struct matchInfo {
    bool status;
    string info;
    Tag tag;
    Var* var;
};

class Parser {
    private:
        Nonterminal* parserRoot;
        list<Token*> tokenList;
        list<Token*>::iterator tokenIterator;
        SymTab symTab;
        //in parserScanner.cpp, is used in parserMatcher.cpp
        bool move();
        Token* scan();
        //in parserMatcher.cpp
        matchInfo program(Nonterminal* father);
        matchInfo segment(Nonterminal* father);
        matchInfo type(Nonterminal* father);
        matchInfo def(Nonterminal* father, bool isExtern, Tag tag);
        matchInfo defdata(Nonterminal* father, bool isExtern, Tag tag);
        matchInfo varrdef(Nonterminal* father, bool isExtern, Tag tag, string id);
        matchInfo init(Nonterminal* father, bool isExtern, Tag tag, string id, bool isPoint);
        matchInfo expr(Nonterminal* father);
        matchInfo iddetail(Nonterminal* father);
        matchInfo deflist(Nonterminal* father, bool isExtern, Tag tag);
        matchInfo fun(Nonterminal* father);
        matchInfo para(Nonterminal* father);
        matchInfo block(Nonterminal* father);
        matchInfo funtail(Nonterminal* father);
        matchInfo idtail(Nonterminal* father, bool isExtern, Tag tag, string id);
        matchInfo paradata(Nonterminal* father);
        matchInfo paradatatail(Nonterminal* father);
        matchInfo paralist(Nonterminal* father);
        matchInfo subprogram(Nonterminal* father);
        matchInfo localdef(Nonterminal* father);
        matchInfo statement(Nonterminal* father);
        matchInfo assexpr(Nonterminal* father);
        matchInfo asstail(Nonterminal* father);
        matchInfo orexpr(Nonterminal* father);
        matchInfo ortail(Nonterminal* father);
        matchInfo andexpr(Nonterminal* father);
        matchInfo andtail(Nonterminal* father);
        matchInfo cmpexpr(Nonterminal* father);
        matchInfo cmptail(Nonterminal* father);
        matchInfo cmps(Nonterminal* father);
        matchInfo aloexpr(Nonterminal* father);
        matchInfo alotail(Nonterminal* father);
        matchInfo adds(Nonterminal* father);
        matchInfo muls(Nonterminal* father);
        matchInfo item(Nonterminal* father);
        matchInfo factor(Nonterminal* father);
        matchInfo itemtail(Nonterminal* father);
        matchInfo lop(Nonterminal* father);
        matchInfo val(Nonterminal* father);
        matchInfo elem(Nonterminal* father);
        matchInfo rop(Nonterminal* father);
        matchInfo idexpr(Nonterminal* father);
        matchInfo literal(Nonterminal* father);
        matchInfo realarg(Nonterminal* father);
        matchInfo arglist(Nonterminal* father);
        matchInfo arg(Nonterminal* father);
        matchInfo altexpr(Nonterminal* father);
        matchInfo whilestat(Nonterminal* father);
        matchInfo forstat(Nonterminal* father);
        matchInfo switchstat(Nonterminal* father);
        matchInfo dowhilestat(Nonterminal* father);
        matchInfo ifstat(Nonterminal* father);
        matchInfo forinit(Nonterminal* father);
        matchInfo elsestat(Nonterminal* father);
        matchInfo casestat(Nonterminal* father);
        matchInfo caselabel(Nonterminal* father);
    public:
        Parser(list<Token*> tokenList) {
            this->tokenList = tokenList;
            this->tokenIterator = this->tokenList.begin();
            parserRoot = new Nonterminal(ROOT);
            initNonterminalMap();
            this->symTab = SymTab();
        }
        //in parserMain.cpp
        Nonterminal *run();
        int print(ParserSign* node, int stack);
};

#endif