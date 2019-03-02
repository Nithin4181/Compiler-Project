#ifndef _PARSER_DEF_H
#define _PARSER_DEF_H

#include "lexerDef.h"

#define NON_TERMINAL_COUNT 46  // TODO
#define TERMINAL_COUNT 55      // 54 token types + $

#define SYMBOL_SIZE 20

// #define BIT_VECTOR_SIZE (sizeof(int)-1+ TOTAL_TERMINALS)/sizeof(int)

typedef Token_type Terminal;

typedef enum{
	// List of non terminals
}NonTerminal;

typedef union{
	Terminal term;
	NonTerminal nonterm;
}Symbol;

typedef enum{
    False,
    True
}boolean;

typedef struct _symbolNode{
	Symbol symbol;
	boolean isTerminal; //1 for Terminal and 0 for Non Terminal
	struct _symbolNode* next; //Pointer to the next node in the list
}SymbolNode;

typedef struct _symbolList{
	SymbolNode* head;
	int length; 
}SymbolList;

typedef struct _rule{
	SymbolList* symbols; //One rule (i.e RHS of a rule) can have terminals and nonterminals both
	struct _rule* next; //Pointer to next Rule for that non-terminal
	int ruleNo;             // TODO: Check if required
}Rule;

typedef struct _rules{
	Rule* head; //A linked list of rules for a non-terminal
	int ruleCount; //Number of rules for a particular non-terminal
}Rules;

typedef struct _grammar{
	int ruleCount;
	Rules** rules; 	
}Grammar;

#endif