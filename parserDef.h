/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#ifndef _PARSER_DEF_H
#define _PARSER_DEF_H

#include <stdbool.h>
#include "lexerDef.h"

#define NON_TERMINAL_COUNT 51  // Number of non-terminals
#define TERMINAL_COUNT 56      // 54 token types + eps + $

#define SYMBOL_SIZE 25          // Max size of non-terminal

typedef Token_type Terminal;

typedef enum{                   // Non terminals in the grammar
	program,
    otherFunctions,
    mainFunction,
    stmts,
    stmt,
    function,
    input_par,
    output_par,
    parameter_list,
    dataType,
    remaining_list,
    primitiveDatatype,
    constructedDatatype,
    typeDefinitions,
    typeDefinition,
    declarations,
    declaration,
    otherStmts,
    returnStmt,
    fieldDefinitions,
    fieldDefinition,
    moreFields,
    global_or_not,
    assignmentStmt,
    iterativeStmt,
    conditionalStmt,
    ioStmt,
    funCallStmt,
    singleOrRecId,
    arithmeticExpression,
    new_24,
    outputParameters,
    inputParameters,
    idList,
    booleanExpression,
    elsePart,
    allVar,
    allVar_1,
    var,
    term,
    expPrime,
    lowPrecedenceOperators,
    factor,
    termPrime,
    highPrecedenceOperators,
    all,
    temp,
    logicalOp,
    relationalOp,
    optionalReturn,
    more_ids
}NonTerminal;

typedef union{                  // Union storing either terminal or non-terminal
	Terminal term;
	NonTerminal nonterm;
}Symbol;

typedef struct _symbolNode{     // Node in the linked list
	Symbol symbol;
	bool isTerminal;            // true for Terminal and false for Non Terminal
	struct _symbolNode* next;   // pointer to the next node in the list
}SymbolNode;

typedef struct _symbolList{     // Linked list of terminals and non-terminals
	SymbolNode* head;
	int length; 
}SymbolList;

typedef struct _rule{           // Rule structure
	SymbolList* symbols;        // Linked list storing rule
	struct _rule* next;         // Pointer to next Rule for that non-terminal
	int ruleNo;                 // Rule no.
}Rule;

typedef struct _rules{          // Linked list of rules
	Rule* head;                 // A linked list of rules for a non-terminal
	int ruleCount;              // Number of rules for a particular non-terminal
}Rules;

typedef struct _grammar{        // Grammar structure
	int ruleCount;              // Number of rules in grammar
	Rules** rules; 	            // Array of pointers to linked list of rules
}Grammar;

typedef struct _firstAndFollow{ // Structure storing first and follow sets
	bool** first;               // 2D binary array for first set
	bool** follow;              // 2D binary array for follow set
}FirstAndFollow;

typedef Rule*** ParsingTable;   // Parsing table is a 2D array of pointers to rules

#endif