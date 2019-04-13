#include <stdio.h>
#include <stdlib.h>

#include "symbolTableDef.h"
// #include "semanticAnalyzerDef.h"

STSymbolTable* newSTSymbolTable(int nSlots);
int getHash(char* key, int nSlots);
void addSTSymbol(STSymbolTable* table, char* key, STSymbol* symbol);
STSymbolNode* getSymbol(char* key, STSymbolTable* lTable);

STTree* newSTTree();
STTreeNode* makeSTTreeNode(STTreeNode* parent, char* fnscope);

STScopeNest* newScopeNest();
void addToSTScopeNest(STTreeNode* parent, STTreeNode* node);

void traverseAST(ASTNode* node, STTreeNode* currScope, ErrorList* errors, int* num);
STTree makeSymbolTables(AST* ast, ErrorList* errors);
STSymbol* makeSTSymbol(ASTNode* node, int num);

STSymbolNode* getInfoFromAST(ASTNode* node);
void setParentFn(ASTNode* node);
int checkRecursion(ASTNode* node); //????

void sortSymbols(STSlotsList* list);
void sortSymbolsR(STSymbolNode* node, int n);

void displaySTTree(STTree* tree);
void displaySTTreeTraversal(STTreeNode* node)




