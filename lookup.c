#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lookup.h"

Lookup* createTable(int slotCount){
    Lookup* lookupTable = (Lookup*)malloc(sizeof(Lookup));
    lookupTable->slotCount = slotCount;
    lookupTable->slots = (List**)malloc(slotCount*sizeof(List*));
    for(int i=0; i<slotCount; ++i){
		lookupTable->slots[i] = (List*)malloc(sizeof(List));
		lookupTable->slots[i]->head = NULL;
	}
    fillTable(lookupTable);
    return lookupTable;
}

void addKeyword(Lookup* lookupTable, char* lexeme, Token_type token){
    int index = hash(lexeme,lookupTable->slotCount);
    Node* node = (Node*)malloc(sizeof(Node));
    node->token = token;
    node->lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme)+1));
    strcpy(node->lexeme,lexeme);
    node->next = lookupTable->slots[index]->head;
    lookupTable->slots[index]->head = node;
}

int hash(char* lexeme, int slotCount){
	int sum = 0;
	for(int i=0; i<strlen(lexeme); ++i){
		sum = (sum*7 + ((int)lexeme[i]))%1000000007;
		if(sum<0)
			sum = 0;
	}
	int hashvalue = sum%slotCount;
	return hashvalue;
}

void fillTable(Lookup* lookupTable){
	addKeyword(lookupTable, "with", TK_WITH);
	addKeyword(lookupTable, "parameters", TK_PARAMETERS);
	addKeyword(lookupTable, "end", TK_END);
	addKeyword(lookupTable, "while", TK_WHILE);
	addKeyword(lookupTable, "type", TK_TYPE);
	addKeyword(lookupTable, "_main", TK_MAIN);
	addKeyword(lookupTable, "global", TK_GLOBAL);
	addKeyword(lookupTable, "parameter", TK_PARAMETER);
	addKeyword(lookupTable, "list", TK_LIST);
	addKeyword(lookupTable, "input", TK_INPUT);
	addKeyword(lookupTable, "output", TK_OUTPUT);
	addKeyword(lookupTable, "int", TK_INT);
    addKeyword(lookupTable, "real", TK_REAL);
	addKeyword(lookupTable, "endwhile", TK_ENDWHILE);
	addKeyword(lookupTable, "if", TK_IF);
    addKeyword(lookupTable, "then", TK_THEN);
	addKeyword(lookupTable, "endif", TK_ENDIF);
	addKeyword(lookupTable, "read", TK_READ);
    addKeyword(lookupTable, "write", TK_WRITE);
	addKeyword(lookupTable, "return", TK_RETURN);
	addKeyword(lookupTable, "call", TK_CALL);
	addKeyword(lookupTable, "record", TK_RECORD);
	addKeyword(lookupTable, "endrecord", TK_ENDRECORD);
	addKeyword(lookupTable, "else", TK_ELSE);
}

Node* getTokenType(char* lexeme, Lookup* lookupTable){
	int index = hash(lexeme, lookupTable->slotCount);
	Node* temp = lookupTable->slots[index]->head;
	while(temp!=NULL){
		if(strcmp(temp->lexeme,lexeme)==0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}