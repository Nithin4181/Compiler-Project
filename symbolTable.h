/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#include <stdio.h>
#include <stdlib.h>

#include "astDef.h"
// #include "semanticAnalyzerDef.h"

STSymbolTable* newSTSymbolTable(int nSlots);
int getHash(char* key, int nSlots);
void addSTSymbol(STSymbolTable* table, char* key, STSymbol* symbol);
STSymbolNode* getSymbol(char* key, STSymbolTable* lTable);

STTree* newSTTree();
STTreeNode* makeSTTreeNode(STTreeNode* parent, char* fnscope);

STScopeNest* newScopeNest();
void addToSTScopeNest(STTreeNode* parent, STTreeNode* node);

void traverseASTPostorder(ASTNode* node, STTreeNode* currScope, int* num, STTree globalTree);
STTree makeSymbolTables(AST ast);
STSymbol* makeSTSymbol(ASTNode* node, int num);

STSymbolNode* getInfoFromAST(ASTNode* node);
void setParentFn(ASTNode* node);
bool checkRecursion(ASTNode* node); 

void sortSymbols(STSlotsList* list);
void sortSymbolsR(STSymbolNode* node, int n);

void displaySTTree(STTree tree);
void displaySTTreeTraversal(STTreeNode* node);

recordDef* makeRecordDef(AST ast);
recordDef* getRecord(char* name, recordTable* rTable);
void addRecordDef(recordTable* table, recordDef* record);

void printGlobalVars(STTree tree);
void printGlobalVarsTraversal(STTreeNode* node);

void printFnMemories(STTree tree);
void printFnMemoriesTraversal(STTreeNode* node);

void printTypeExpressionGlobalRecord(STTree tree);
void printRecordDefs();