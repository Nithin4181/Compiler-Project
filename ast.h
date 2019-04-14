/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#ifndef _AST_H
#define _AST_H

#include <stdio.h>
#include <stdlib.h>

#include "astDef.h"

AST makeAST(ParseTree pt);

void makeAST_postOrderTraversal(ParseTree pt);

void arithmeticASTTraversal(ParseTree pt, ParseTree predecessor);

ASTChildren* initializeASTChildren();

void addASTChildren(ASTChildren *ch, AST ast);

ASTNode* makeASTNode(astNodeName label, bool leaf, ASTNode* parent, ASTChildren* children, ASTNode* next, Lexical_Unit* lu, bool global);

ASTNode* makeASTLeaf(Lexical_Unit* lu, bool global);

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