#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H
#define LEXERROR(code) lexError(code)
using namespace std;
//error type
enum LexError {
    STR_NO_R_QUTION,
    NUM_BIN_TYPE,
    NUM_HEX_TYPE,
    CHAR_NO_R_QUTION,
    CHAR_NO_DATA,
    OR_NO_PAIR,
    COMMENT_NO_END,
    TOKEN_NO_EXIST
};
//print error
void lexError(int code);
int getErrorNum();
#endif