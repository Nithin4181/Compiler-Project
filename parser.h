#ifndef _PARSER_H
#define _PARSER_H

#include "parserDef.h"
#include "stack.h"
#include "NaryTree.h"

Grammar* loadGrammar(char* inputFile);

SymbolNode* makeSymbolNode(int index, bool isTerminal);

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

ParsingTable makeNewParseTable();

void createParseTable(Grammar* grammar, FirstAndFollow* sets, ParsingTable table);

void printParsingTable(ParsingTable table);

ParseTree  parseInputSourceCode(char *testcaseFile, ParsingTable table, FirstAndFollow* ff, int* parseErrors);

void printParseTree(ParseTree tree, char* fname);

void printParseTree_util(TreeNode* node, FILE** fp1);

void printNode(FILE** fp, TreeNode* node, bool isLeaf, char* empty, char* no, char* yes, char* root);

void printTopOfStack(TreeNode* treeNode);

void printChildren(Children* children);

#endif