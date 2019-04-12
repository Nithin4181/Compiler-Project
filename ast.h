#ifndef _AST_H
#define _AST_H

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
    ID_LIST
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

	struct symTableTreeNode* currentScope;
    struct ASTNode* extend;

	Token_type token;

	struct typeList* returnTypes;

	int lineNo;
	Val* value;
}ASTNode;

typedef ASTNode* AST;

typedef struct _typeList{
    struct _typeListNode* head;
    struct _typeListNode* tail;
    int noSiblings;
}typeList;

typedef struct _typeListNode{
    struct typeListNode* next;
    Token_type type;
}typeListNode;

AST makeAST(ParseTree pt);

void makeAST_postOrderTraversal(ParseTree pt);

ASTChildren* initializeASTChildren();

void addASTChildren(ASTChildren *ch,AST * ast);

ASTNode* makeASTNode(astNodeName label, bool leaf, ASTNode* parent, ASTChildren* children, ASTNode* next, Lexical_Unit* lu);

inline ASTNode* makeASTLeaf(Lexical_Unit* lu);

void addParentPointers(ASTNode* parent, ASTChildren* children);

typeList * initializeTypeList();

void addReturnType(typeList *tl, Token_type type);

void makeAST_nodeTraversal(ParseTree pt);

void compressionRatio(ParseTree pt, AST ast);

void getASTSize(AST ast, int* count, int* size);

void getParseTreeSize(ParseTree pt, int* count, int* size);

void displayAST(AST ast);

void displayPreOrderAST(AST ast);

#endif