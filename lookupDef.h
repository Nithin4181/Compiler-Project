#ifndef _LOOKUP_DEF_H
#define _LOOKUP_DEF_H

#include "lexerDef.h"

typedef struct node{
	Token_type token;
	char* lexeme;
	struct node* next;
}Node;


typedef struct list{
	Node* head;
}List;

typedef struct lookup{
	int slotCount;
	List** slots;
}Lookup;

#endif