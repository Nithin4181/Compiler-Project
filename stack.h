/*
Group No: 26
Authors:
Naveen Unnikrishnan - 2016A7PS0111P
Adithya Mamallan - 2016A7PS0028P
Nithin Myppan Benny - 2016A7PS0014P
Swarup N - 2016A7PS0080P
*/
#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NaryTree.h"

typedef struct _stackNode{
	TreeNode* parseTreeNode;
	struct _stackNode* next;
}StackNode;

// typedef StackNode* Stack;

typedef struct _stack{
    StackNode* head;
    int size;
}Stack;

Stack* initializeStack();

StackNode* createStackNode(TreeNode* treeNode);

StackNode* pop(Stack* stack);

StackNode* top(Stack* stack);

void push(Stack* stack, TreeNode* treeNode);

void pushChildrenToStack(Stack* stack, TreeNode* treeNode);

bool bottomOfStack(Stack* stack);

#endif