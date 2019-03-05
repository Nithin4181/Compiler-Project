/*
Group No: 26
Authors:
Naveen Unnikrishnan - 2016A7PS0111P
Adithya Mamallan - 2016A7PS0028P
Nithin Myppan Benny - 2016A7PS0014P
Swarup N - 2016A7PS0080P
*/
#include "stack.h"

Stack* initializeStack(){
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->size=0;
	stack->head = NULL;
	return stack;
}

StackNode* createStackNode(TreeNode* treeNode){
	StackNode* node = (StackNode*)malloc(sizeof(StackNode));	
	node->parseTreeNode = treeNode;
	node->next = NULL;
	return node;
}

void push(Stack* stack, TreeNode* treeNode){
	StackNode* node = createStackNode(treeNode);
	node->next = stack->head;
	stack->head = node; 
	stack->size++;
}

StackNode* pop(Stack* stack){
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
	if(stack->size == 0){
		printf("Stack is Empty\n");
		return NULL;
	}
	else{
		return stack->head;
	}
}

void pushChildrenToStack(Stack* stack, TreeNode* treeNode){
    // Push children to stack in reverse order
	Children* children = treeNode->children;
	TreeNode* temp = children->head;

	//DUMMY NODE
	StackNode* head = createStackNode(NULL);
	StackNode* temp_stack = head;
	
	//Don't Push EPSILON on the stack
	if(temp->content->isTerminal && temp->content->type.term == EPS){
		return;
	}

	while(temp!=NULL){
		//Create a Stack Node
		StackNode* node = createStackNode(temp);
		
		temp_stack->next = node;
		temp_stack = temp_stack->next;

		temp = temp->next;
	}

	temp_stack->next = stack->head;
	stack->head = head->next;

	stack->size += treeNode->children->siblingCount;
}

//Whether $ is on the top of the stack or not
bool bottomOfStack(Stack* stack){
	if(stack->size == 1){
		NodeContent* content = stack->head->parseTreeNode->content;
		if(content->isTerminal && content->type.term == DOLLAR)
			return true;
	}		
	return false;
}