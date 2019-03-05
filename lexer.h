/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexerDef.h"
#include "lookup.h"

Lexical_Unit* getNextTokenUtil(FILE** fp, bool printErrors);    // Return next token from file. printErrors argument enable/disables lexical error printing.

Lexical_Unit* getNextToken(FILE** fp);  // Return next token with error printing enabled

Lexical_Unit* getNextToken_NoError(FILE** fp);  // Return next token with error printing disabled

FILE* getStream(FILE* fp);  // Read chunk of data from source code file to buffer

void addToken(Lexical_Unit* lu, Token_type type, char* lexeme, Val* value);  // Populate tokenInfo (lexical unit) with given data

void removeComments(char *testcaseFile);    // Plugin to remove comments

FILE *lexer_initialisation(char *sourceFile);   // Initialize lexer

void printTokenList(char *sourceFile);  // Print list of tokens in source code file

#endif