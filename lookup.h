#ifndef _LOOKUP_H
#define _LOOKUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lookupDef.h"

Lookup* createTable(int slotCount);

void addKeyword(Lookup* lookupTable, char* key, Token_type token);

int hash(char* key, int slotCount);

void fillTable(Lookup* lookupTable);

Node* getTokenType(char* lexeme, Lookup* lookupTable);

#endif