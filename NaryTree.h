/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#ifndef _NARYTREE_H
#define _NARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parserDef.h" 

typedef struct _nodeContent{
	bool isTerminal;
	Symbol type;
}NodeContent;

typedef struct _linkedList{
	struct _treeNode* head;
	int siblingCount;
}Children;

typedef struct _treeNode{
	struct _treeNode* next;
	NodeContent* content;
	Lexical_Unit* lu;      
	Children* children;		
	struct _treeNode* parent;
	int ruleNo;
}TreeNode;

typedef TreeNode* ParseTree;

Children* initializeChildren();

TreeNode* makeNode(bool isTerminal,	int index, TreeNode* parent);

void addChildren(TreeNode* node, Rule* rule);

#endif