/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#ifndef _AST_DEF_H
#define _AST_DEF_H

#include "NaryTree.h"
#include "symbolTableDef.h"

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
    RECORD
}astNodeName;

typedef struct astChildren{
	struct astNode* head;
	int noSiblings;
	struct astNode* tail;
}ASTChildren;

typedef struct astNode{
	astNodeName label;
	bool leaf;
	struct astNode* parent;
	ASTChildren* children;
	struct astNode* next; 
	Lexical_Unit* lu; 

	STTreeNode* currentScope;
    struct astNode* extend;

	Token_type token;

	struct _typeList* returnTypes;
    struct astNode* callingFunction;

	int lineNo;
	Val* value;
    bool global;
}ASTNode;

typedef ASTNode* AST;

typedef struct _typeList{
    struct _typeListNode* head;
    struct _typeListNode* tail;
    int noSiblings;
}typeList;

typedef struct _typeListNode{
    struct _typeListNode* next;
    Token_type type;
}typeListNode;

#endif