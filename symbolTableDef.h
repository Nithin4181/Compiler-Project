#ifndef SYMBOLTABLEDEF_H
#define SYMBOLTABLEDEF_H

#include <stdio.h>
#include <stdlib.h>
#include "astDef.h"

#define NO_OF_SLOTS 32

typedef struct{
    Token* lexToken;
    int datatype;
    int offset;
    int width;
    bool isAssigned;
    ASTNode* ASTNode;
    int number;
    int* val;
}STSymbol;

struct _stsymbolnode{
    STSymbol* symbol;
    struct _stsymbolnode* next;
};

typedef struct _stsymbolnode STSymbolNode;

typedef struct{
    STSymbolNode* head;
    int nSymbols;
}STSlotsList;

typedef struct{
    int nSlots;
    STSlotsList** slots;
}STSymbolTable;

struct _sttreenode{
    STSymbolTable* table;
    STScopeNest* children;
    char* fnscope;
    struct _sttreenode* next;
    struct _sttreenode* parent;
};

typedef struct _sttreenode STTreeNode;

typedef struct{
    STTreeNode head;
    int nSiblings;
}STScopeNest;

typedef struct{
    STTreeNode* root;
}STTree;

#endif
