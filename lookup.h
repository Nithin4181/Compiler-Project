/*
Group No: 26
Authors:
Naveen Unnikrishnan - 2016A7PS0111P
Adithya Mamallan - 2016A7PS0028P
Nithin Myppan Benny - 2016A7PS0014P
Swarup N - 2016A7PS0080P
*/
#ifndef _LOOKUP_H
#define _LOOKUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


Lookup* createTable(int slotCount);

void addKeyword(Lookup* lookupTable, char* key, Token_type token);

int hash(char* key, int slotCount);

void fillTable(Lookup* lookupTable);

Node* getTokenType(char* lexeme, Lookup* lookupTable);

#endif