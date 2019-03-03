#ifndef _PARSER_H
#define _PARSER_H

#include "parserDef.h"

Grammar* loadGrammar(char* inputFile);

SymbolNode* makeSymbolNode(int enum_int, int term_or_nonterm);

int find(char* str, bool isTerminal);

SymbolNode* addToRule(SymbolList* list, SymbolNode* symbolNode, SymbolNode* currentNode);

FirstAndFollow* getFirstAndFollowSets(Grammar* grammar);

void printGrammar(Grammar* grammar);

void computeFirstSet(Grammar* grammar, NonTerminal nonTerminal, bool** first);

void addToSet(bool* set, int index);

bool check(bool *set, int index);

bool setUnion(bool* a, bool* b);

void computeFollowSet(Grammar* grammar, FirstAndFollow* sets);

bool computeFollowUtil(Grammar* grammar, bool** first, bool** follow);

void printFirstAndFollow(FirstAndFollow* sets);

void printSet(bool* set);

#endif