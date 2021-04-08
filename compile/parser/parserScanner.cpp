/**
 * 语义分析的扫描器
 **/
#include "parser.h"
using namespace std;
Token* Parser::move() {
    iterator++;
    return *iterator;
}
Token* Parser::scan() {
    return *iterator;
}