#define SEMERROR(code, name) semError(code, name)
#define SEMWARN(code, name) semWarn(code, name)
using namespace std;
enum SemError{
    VAR_RE_DEF,
    FUN_RE_DEF,
    VAR_UN_DEC,
    FUN_UN_DEC,
    FUN_DEC_ERR,
    FUN_CALL_ERR,
    DEC_INIT_DENY,
    EXTERN_FUN_DEF,
    ARRAY_LEN_INVALID,
    VAR_INIT_ERR,
    GLB_INIT_ERR,
    VOID_VAR,
    EXPR_NOT_LEFT_VAL,
    ASSIGN_TYPE_ERR,
    EXPR_IS_BASE,
    EXPR_NOT_BASE,
    ARR_TYPE_ERR,
    EXPR_IS_VOID,
    BREAK_ERR,
    CONTINUE_ERR,
    RETURN_ERR
};
enum SemWarn{
    FUN_DEC_CONFLICT,
    FUN_RET_CONFLICT
};
void semError(int code, string name);
void semWarn(int code, string name);