#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"
#include "semanticAnalyzer.h"

STSymbolTable* newSTSymbolTable(int nSlots){
    STSymbolTable* table = (STSymboltable*) malloc(sizeof(STSymbolTable));
    table->nSlots = nSlots;
    table->slots = (STSlotsList**) malloc(nSlots*sizeof(STSlotsList*));

    for(int i=0; i<nSlots; ++i){
        table->slots[i] = (STSlotsList*) malloc(sizeof(STSlotsList));
        table->slots[i]->head = NULL;
        table->slots[i]->nSymbols = 0;
    }

    return table;
}


int findHash(char* key, int nSlots){
    int sum = 0;
    for(int i=0; i<strlen(key); ++i){
        sum = sum*10 + (int)(key[i] - '0');
        sum = sum%nSlots;
    }
    int hashval = sum%nSlots;
    return hashval;
}

void addSTSymbol(STSymbolTable table, char* key, Symbol* symbol){
    int k = findHash(key, table->nSlots);
    STSymbolNode* node = (STSymbolNode*) malloc(sizeof(STSymbolNode));
    node->symbol = symbol;
    node->next = table->slots[k]->head;
    table->slots[k]->head = node;
    table->slots[k]->nSymbols++;
}

STSymbolNode* getSymbol(char* key, SymbolTable* lTable){
    int k = findHash(key, lTable->nSlots);
    STSymbolNode* temp = lTable->slots[k]->head;
    while(temp!=NULL){
        if(strcmp(temp->symbol->lexToken->lexeme, key)==0) return temp;
        temp = temp->next;
    }
    return NULL;
}

STTree* newSTTree(){
    STTree* tree = (STTree*) malloc(sizeof(STTree));
    tree->root = NULL;
    return tree;
}

STTreeNode* makeSTTreeNode(STTreeNode* parent, char* fnscope){
    STTreeNode* node = (STTreeNode*) malloc(sizeof(STTreeNode));
    node->table = newSTSymbolTable(NO_OF_SLOTS);
    node->next = NULL;
    node->children = newScopeNest();
    node->parent = parent;
    node->fnscope = fnscope;
    if(parent!=NULL){
        addToSTScopeNest(parent, node);
    }
    return node;
}


STScopeNest* newScopeNest(){
    STScopeNest* nest = (STScopeNest*) malloc(sizeof(STScopeNest));
    nest->head = NULL;
    nest->nSiblings = 0;
    return nest;
}

void addToSTScopeNest(STTreeNode* parent, STTreeNode* node){
    node->next = parent->children->head;
    parent->children->head = node;
    parent->children->nSiblings++;
    return children;
}

void traverseAST(ASTNode* node, STTreeNode* currScope, ErrorList* errors, int* num){
    //maybe rewrite
    if(node==NULL) return;
    node->currScope = currScope;
}

STTree* makeSymbolTables(AST* ast, ErrorList* errors){
    STTree* tree = newSTTree();
    char* mainscope = (char*)malloc(sizeof(char)*8);  //can be 6, I guess?
    strcpy(mainscope, "_main");
    tree->root = makeSTTreeNode(NULL, mainscope);
    int num = 0;

}

STSymbol* makeSTSymbol(ASTNode* node, int num){
    STSymbol* symbol = (STSymbol*) malloc(sizeof(STSymbol));
    symbol->lexToken = node->lexToken; //AST
    symbol->offset = 0;
    symbol->width = 0;
    symbol->number = number;
    symbol->ASTNode = node;
    if(symbol->lexToken->type!=TK_FUNID){
        symbol->datatype = node->parent->children->head->lexToken->type; //AST
    }
    else symbol->datatype = TK_FUNID;

    if(symbol->datatype==TK_INT) symbol->width = 2;
    else if(symbol->datatype==TK_REAL) symbol->width = 4;
    symbol->isAssigned = false;
    symbol->val = NULL;
    return symbol;
}

STSymbolNode* getInfoFromAST(ASTNode* node);
void setParentFn(ASTNode* node);
int checkRecursion(ASTNode* node); //????

void printSTSymbol(STSymbol* Symbol, bool isNested, char* fnscope, char* parent);
void printSTSymbolTable(STTreeNode* node, bool isNested);
void sortSymbols(STSlotsList* list);
void sortSymbolsR(STSymbolNode* node, int n);
void printSTTree(STTree* tree);
void printSTTreeR(STTreeNode* node, bool isNested);
