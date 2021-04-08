#ifndef PARSERERRORHANDLER_H
#define PARSERERRORHANDLER_H
#include <iostream>
using namespace std;

enum parserErrorType {
    //TODO
};

class ParserErrorHandler {
    private:
        parserErrorType type;
        char* parserErrorMap;
    public:
        ParserErrorHandler(parserErrorType type) {
            this->type = type;
            this->parserErrorMap = {
                //TODO
            };
        }
        void printError() {
            printf("error: %s", parserErrorMap[(int)type]);
        }
};

#endif