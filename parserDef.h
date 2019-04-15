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

#define NON_TERMINAL_COUNT 51   non-terminals
#define TERMINAL_COUNT 56      types + eps + $

#define SYMBOL_SIZE 25          of non-terminal

typedef Token_type Terminal;

typedef enum{                   nals in the grammar
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

typedef union{                  
	Terminal term;
	NonTerminal nonterm;
}Symbol;

typedef struct _symbolNode{     
	Symbol symbol;
	bool isTerminal;            
	struct _symbolNode* next;   
}SymbolNode;

typedef struct _symbolList{     
	SymbolNode* head;
	int length; 
}SymbolList;

typedef struct _rule{           
	SymbolList* symbols;        
	struct _rule* next;         
	int ruleNo;
}Rule;              

typedef struct _rules{          
	Rule* head;                 
	int ruleCount;              
}Rules;

typedef struct _grammar{        
	int ruleCount;              
	Rules** rules; 	            
}Grammar;

typedef struct _firstAndFollow{  
	bool** first;                
	bool** follow;               
}FirstAndFollow;

typedef Rule*** ParsingTable;   

#endif