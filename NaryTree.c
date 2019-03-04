#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "NaryTree.h"

Children* initializeChildren(){
	Children* children = (Children*)malloc(sizeof(Children));
	children->head = NULL;
	children->siblingCount = 0;
	return children;
}

TreeNode* makeNode(bool isTerminal,	int index, TreeNode* parent){
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->children = NULL;
	node->lu = NULL;
	node->next = NULL;
	node->content = (NodeContent*)malloc(sizeof(NodeContent));
	node->content->isTerminal = isTerminal;
	//Storing which terminal or non-terminal the node represents
	Symbol type;
	//Terminal
	if(isTerminal)
		type.term = (Token_type)index;
	//Non Terminal
	else
		type.nonterm = (NonTerminal)index;
	node->content->type = type;
	node->parent = parent;
	node->ruleNo = 0;
	return node;
}

void addChildren(TreeNode* node, Rule* rule){
	node->ruleNo = rule->ruleNo;
	Children* children = initializeChildren();
    TreeNode* temp = children->head;
	SymbolList* symbols = rule->symbols;
	SymbolNode* temp2 = symbols->head;

	for(int k = 0; k < symbols->length; ++k){
		TreeNode* newNode;
		//Terminal
		if(temp2->isTerminal){
			newNode = makeNode(temp2->isTerminal, temp2->symbol.term, node);
		}
		//Non Terminal
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
		//Go to the next symbol in the rule
		temp2 = temp2->next;
		//Increase the number of siblings
		children->siblingCount++;
	}
	//Assigning the children to the current node
	node->children = children;
}