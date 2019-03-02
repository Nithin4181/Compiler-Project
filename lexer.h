#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "lookup.h"

Lexical_Unit* getNextToken(FILE** fp);

FILE* getStream(FILE* fp);

void addToken(Lexical_Unit* token, Token_type type, char* lexeme, Val* value);

void removeComments(char *testcaseFile, char* cleanFile);

FILE *lexer_initialisation(char *sourceFile);

void printTokenList(char *sourceFile);

#endif