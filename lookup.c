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
#include "lookup.h"

Lookup* createTable(int slotCount){
    /*Description: Create an empty Lookup Table*/
    /*Arguments: Number of slots for hashing*/
	/*Return type: Pointer to lookup table*/	
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
    /* Description: Adds the (lexeme,token) pair to the lookup table */
    /* Arguments: Pointer to lookup table, lexeme, token */
    /* Return type: void */	
    int index = hash(lexeme,lookupTable->slotCount);
    Node* node = (Node*)malloc(sizeof(Node));
    node->token = token;
    node->lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme)+1));
    strcpy(node->lexeme,lexeme);
    node->next = lookupTable->slots[index]->head;
    lookupTable->slots[index]->head = node;
}

int hash(char* lexeme, int slotCount){
    /* Description: Get hash value for a lexeme to index into lookup table */
    /* Arguments: lexeme, number of slots in lookup table */
	/* Return type: int */	
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
    /* Description: Fill table with language specific keywords */
    /* Arguments: Pointer to lookup table */
	/* Return type: void */	
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
    /* Description: Do a keyword lookup in the lookup table */
    /* Arguments: lexeme, pointer to lookup table */
	/* Return type: Node pointer if lexeme is a keyword. NULL otherwise. */	
	int index = hash(lexeme, lookupTable->slotCount);
	Node* temp = lookupTable->slots[index]->head;
	while(temp!=NULL){
		if(strcmp(temp->lexeme,lexeme)==0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}