#ifndef _PARSER_H
#define _PARSER_H

#include "parserDef.h"

Grammar* loadGrammar(char* inputFile);

SymbolNode* makeSymbolNode(int enum_int, int term_or_nonterm);

int find(char* str, int term_or_nonterm);

SymbolNode* addToRule(SymbolList* list, SymbolNode* symbolNode, SymbolNode* currentNode);

void printGrammar(Grammar* grammar);

#endif