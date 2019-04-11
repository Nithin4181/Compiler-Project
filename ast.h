
#ifndef AST
#define AST

#include <stdio.h>
#include <stdlib.h>

#include "NaryTree.h"


typedef enum{
    PROGRAM,
    MAIN,
    OTHER_FN,
    FN,
    PAR_LIST,
    STMTS,
    TYPE_DEFS,
    TYPE_DEF,
    FIELD_DEFS,
    FIELD_DEF,
    MORE_FIELDS,
    DECLARATIONS,
    DECLARATION,
    OTHER_STMTS,
    ASSIGNMENT_STMT,
    SINGLE_OR_REC_ID,
    FUN_CALL_STMT,
    ITERATIVE_STMT,
    CONDITIONAL_STMT,
    ELSE,
    IO_STMT_READ,
    IO_STMT_WRITE,
    ALL_VAR,
    ARITHMETIC,
    EXP_PRIME,
    TERM,
    TERM_PRIME,
    ALL,
    BOOLEAN1,
    BOOLEAN2,
    BOOLEAN3,
    RETURN,
    ID_LIST,
}astNodeName;










#endif