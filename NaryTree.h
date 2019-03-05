/*
Group No: 26
Authors:
Naveen Unnikrishnan - 2016A7PS0111P
Adithya Mamallan - 2016A7PS0028P
Nithin Myppan Benny - 2016A7PS0014P
Swarup N - 2016A7PS0080P
*/
#ifndef _NARYTREE_H
#define _NARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parserDef.h" 

//Information about the tree Node
typedef struct _nodeContent{
	bool isTerminal;
	Symbol type;
}NodeContent;

//Linked List of siblings
typedef struct _linkedList{
	struct _treeNode* head;
	int siblingCount;
}Children;

typedef struct _treeNode{
	struct _treeNode* next; // The node itself is a part of some linked list

	NodeContent* content;
	Lexical_Unit* lu;       // If it's a leaf node

	//Linked list of Children
	Children* children;	

	struct _treeNode* parent; //Pointer to parent node
	int ruleNo;
}TreeNode;

typedef TreeNode* ParseTree;


Children* initializeChildren();

TreeNode* makeNode(bool isTerminal,	int index, TreeNode* parent);

void addChildren(TreeNode* node, Rule* rule);

#endif