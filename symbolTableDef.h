#ifndef _SYMBOLTABLE_DEF_H
#define _SYMBOLTABLE_DEF_H

#include <stdio.h>
#include <stdlib.h>
#include "astDef.h"
#include "lexerDef.h"

#define NO_OF_SLOTS 32

typedef struct{
    Lexical_Unit* lu;
    int datatype;
    int offset;
    int width;
    bool isAssigned;
    struct astNode* ASTNode;
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

typedef struct _stscopenest{
    struct _sttreenode* head;
    int nSiblings;
}STScopeNest;

typedef struct _sttreenode{
    STSymbolTable* table;
    struct _stscopenest* children;
    char* fnscope;
    struct _sttreenode* next;
    struct _sttreenode* parent;
}STTreeNode;

typedef STTreeNode* STTree;
// typedef struct{
//     STTreeNode* root;
// }STTree;

#endif
