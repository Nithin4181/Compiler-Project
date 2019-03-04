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