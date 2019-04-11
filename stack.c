/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#include "stack.h"

Stack* initializeStack(){
    /* Description: Initialize stack */
    /* Arguments: void */
    /* Return Type: Pointer to created empty stack */    	
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->size=0;
	stack->head = NULL;
	return stack;
}

StackNode* createStackNode(TreeNode* treeNode){
    /* Description: Wrap node of parse tree into a stack node */
    /* Arguments: Node of parse tree */
    /* Return Type: Stack node */    	
	StackNode* node = (StackNode*)malloc(sizeof(StackNode));	
	node->parseTreeNode = treeNode;
	node->next = NULL;
	return node;
}

void push(Stack* stack, TreeNode* treeNode){
    /* Description: Push parse tree node to stack */
    /* Arguments: Pointers to stack and node to be pushed */
    /* Return Type: void */    	
	StackNode* node = createStackNode(treeNode);
	node->next = stack->head;
	stack->head = node; 
	stack->size++;
}

StackNode* pop(Stack* stack){
    /* Description: Pop out a node from the top of the stack */
    /* Arguments: Pointer to stack */
    /* Return Type: Popped out node */    	
	if(stack->size == 0){
		printf("Stack is Empty\n");
		return NULL;
	}
	else{
		StackNode* temp = stack->head;
		stack->head = stack->head->next;
		stack->size--;
		return temp;
	}
}

StackNode* top(Stack* stack){
    /* Description: Return node at the top of the stack */
    /* Arguments: Pointer to stack */
    /* Return Type: Pointer to node at the top of stack */    	
	if(stack->size == 0){
		printf("Stack is Empty\n");
		return NULL;
	}
	else{
		return stack->head;
	}
}

void pushChildrenToStack(Stack* stack, TreeNode* treeNode){
    /* Description: Push the children of a parse tree node to the stack in reverse order */
    /* Arguments: Pointers to the stack and node of parse tree */
    /* Return Type: void */    	
	Children* children = treeNode->children;
	TreeNode* temp = children->head;

	StackNode* head = createStackNode(NULL);
	StackNode* temp_stack = head;
	
	if(temp->content->isTerminal && temp->content->type.term == EPS){
		return;
	}

	while(temp!=NULL){
		StackNode* node = createStackNode(temp);
		
		temp_stack->next = node;
		temp_stack = temp_stack->next;

		temp = temp->next;
	}

	temp_stack->next = stack->head;
	stack->head = head->next;

	stack->size += treeNode->children->siblingCount;
}

bool bottomOfStack(Stack* stack){
    /* Description: Check if all terminals/non-terminals have been popped out, i.e. $ is at the bottom of stack */
    /* Arguments: Pointer to stack */
    /* Return Type: boolean value */    	
	if(stack->size == 1){
		NodeContent* content = stack->head->parseTreeNode->content;
		if(content->isTerminal && content->type.term == DOLLAR)
			return true;
	}		
	return false;
}