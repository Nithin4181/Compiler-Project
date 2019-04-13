#ifndef _AST_H
#define _AST_H

#include <stdio.h>
#include <stdlib.h>

#include "astDef.h"

AST makeAST(ParseTree pt);

void makeAST_postOrderTraversal(ParseTree pt);

ASTChildren* initializeASTChildren();

void addASTChildren(ASTChildren *ch,AST * ast);

ASTNode* makeASTNode(astNodeName label, bool leaf, ASTNode* parent, ASTChildren* children, ASTNode* next, Lexical_Unit* lu, bool global);

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