#ifndef LEXICAL_H
#define LEXICAL_H
#include <unordered_map>
#include <list>
using namespace std;

enum Tag{
    ERR, END, 
    ID, 
    KW_INT, KW_CHAR, KW_VOID,
    KW_EXTERN, 
    NUM, CH, STR, 
    NOT, LEA, 
    ADD, SUB, MUL, DIV, MOD, 
    INC, DEC, 
    GT, GE, LT, LE, EQU, NEQU, 
    AND, OR, 
    LPAREN, RPAREN,     
    LBRACK, RBRACK, 
    LBRACE, RBRACE, 
    COMMA, COLON, SEMICON, 
    ASSIGN, 
    KW_IF, KW_ELSE, 
    KW_SWITCH, KW_CASE, KW_DEFAULT, 
    KW_WHILE, KW_DO, KW_FOR, 
    KW_BREAK, KW_CONTINUE, KW_RETURN
};

class Token {
    public:
        Tag tag;
        Token() {}
        Token(Tag t) {
            this->tag = t;
        }
        virtual string toString();
        virtual ~Token() {}
};
class Id: public Token {
    public:
        string name;
        Id(string n) {
            this->name = n;
        }
        virtual string toString() {
            return "id:" + name;
        }
};

class Num: public Token {
    public:
        int val;
        Num(int v) {
            this->val = v;
        }
        virtual string toString() {
            return "num:" + to_string(val);
        }
};

class Char: public Token {
    public:
        char ch;
        Char(char c) {
            this->ch = c;
        }
        virtual string toString() {
            string s = "char:";
            s.push_back(ch);
            return s;
        }
};

class Str: public Token {
    public:
        string str;
        Str(string s) {
            this->str = s;
        }
        virtual string toString() {
            return "str:" + str;
        }
};

class Keywords {
    struct string_hash {
        size_t operator() (const string& str) const {
            hash<string> hash_string;
            return hash_string(str.c_str());
            // return __gnu_cxx::__stl_hash_string(str.c_str());
        }
    };
    unordered_map <string, Tag, string_hash> keywords;
    public:
        Keywords(){
            keywords["int"] = KW_INT;
            keywords["char"] = KW_CHAR;
            keywords["void"] = KW_VOID;
            keywords["extern"] = KW_EXTERN;
            keywords["if"] = KW_IF;
            keywords["else"] = KW_ELSE;
            keywords["switch"] = KW_SWITCH;
            keywords["case"] = KW_CASE;
            keywords["default"] = KW_DEFAULT;
            keywords["while"] = KW_WHILE;
            keywords["do"] = KW_DO;
            keywords["for"] = KW_FOR;
            keywords["break"] = KW_BREAK;
            keywords["continue"] = KW_CONTINUE;
            keywords["return"] = KW_RETURN;
        }
        Tag getTag(string name){
            return keywords.find(name) != keywords.end()
                ? keywords[name] : ID;
        }
};

list<Token*> lexical_analyze();

#endif