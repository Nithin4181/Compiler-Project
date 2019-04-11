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
#include <string.h>
#include <math.h>

#include "NaryTree.h"

Children* initializeChildren(){
    /* Description: Initializes children structure */
    /* Arguments: void */
    /* Return Type: Children */	
	Children* children = (Children*)malloc(sizeof(Children));
	children->head = NULL;
	children->siblingCount = 0;
	return children;
}

TreeNode* makeNode(bool isTerminal,	int index, TreeNode* parent){
    /* Description: Create n-ary tree node with given values */
    /* Arguments: bool to check terminal, enum value of terminal/non-terminal, and parent node */
    /* Return Type: Newly created tree node */	
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->children = NULL;
	node->lu = NULL;
	node->next = NULL;
	node->content = (NodeContent*)malloc(sizeof(NodeContent));
	node->content->isTerminal = isTerminal;
	Symbol type;
	if(isTerminal)
		type.term = (Token_type)index;
	else
		type.nonterm = (NonTerminal)index;
	node->content->type = type;
	node->parent = parent;
	node->ruleNo = 0;
	return node;
}

void addChildren(TreeNode* node, Rule* rule){
    /* Description: Adds children to the tree based on grammar rule used */
    /* Arguments: Tree node pointer and rule to be used */
    /* Return Type: void */	
	node->ruleNo = rule->ruleNo;
	Children* children = initializeChildren();
    TreeNode* temp = children->head;
	SymbolList* symbols = rule->symbols;
	SymbolNode* temp2 = symbols->head;
	for(int k = 0; k < symbols->length; ++k){
		TreeNode* newNode;
		if(temp2->isTerminal){
			newNode = makeNode(temp2->isTerminal, temp2->symbol.term, node);
		}
		else{
			newNode = makeNode(temp2->isTerminal, temp2->symbol.nonterm, node);
		}
		if(temp == NULL){
			temp = newNode;
			children->head = temp;
		}
		else{
			temp->next = newNode;
			temp = temp->next;
		}		
		temp2 = temp2->next;
		(children->siblingCount)++;
	}
	node->children = children;
}