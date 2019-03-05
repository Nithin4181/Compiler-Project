/*
Group No: 26
Authors:
Naveen Unnikrishnan - 2016A7PS0111P
Adithya Mamallan - 2016A7PS0028P
Nithin Myppan Benny - 2016A7PS0014P
Swarup N - 2016A7PS0080P
*/
#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexerDef.h"
#include "lookup.h"

Lexical_Unit* getNextTokenUtil(FILE** fp, bool printErrors);

Lexical_Unit* getNextToken(FILE** fp);

Lexical_Unit* getNextToken_NoError(FILE** fp);

FILE* getStream(FILE* fp);

void addToken(Lexical_Unit* token, Token_type type, char* lexeme, Val* value);

void removeComments(char *testcaseFile);

FILE *lexer_initialisation(char *sourceFile);

void printTokenList(char *sourceFile);

#endif