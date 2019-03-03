#include "parser.h"

char* terminalMap[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RECORDID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_ERROR"
};

char* nonTerminalMap[] = {
	"program",
    "otherFunctions",
    "mainFunction",
    "stmts",
    "stmt",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "remaining_list",
    "primitiveDatatype",
    "constructedDatatype",
    "typeDefinitions",
    "typeDefinition",
    "declarations",
    "declaration",
    "otherStmts",
    "returnStmt",
    "fieldDefinitions",
    "fieldDefinition",
    "moreFields",
    "global_or_not",
    "assignmentStmt",
    "iterativeStmt",
    "conditionalStmt",
    "ioStmt",
    "funCallStmt",
    "singleOrRecId",
    "arithmeticExpression",
    "new_24",
    "outputParameters",
    "inputParameters",
    "idList",
    "booleanExpression",
    "elsePart",
    "allVar",
    "allVar_1",
    "var",
    "term",
    "expPrime",
    "lowPrecedenceOperators",
    "factor",
    "termPrime",
    "highPrecedenceOperators",
    "all",
    "temp",
    "logicalOp",
    "relationalOp",
    "optionalReturn",
    "more_ids"
};

Grammar* loadGrammar(char* inputFile){

	int ruleNo = 1;

	FILE* input = fopen(inputFile, "r");

	if(input == NULL){
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	boolean isFileEnd = False;

    Grammar* grammar = (Grammar*)malloc(sizeof(Grammar));
	grammar->ruleCount = 0;
	grammar->rules = (Rules**)malloc(sizeof(Rules*)*NON_TERMINAL_COUNT);
	for(int i = 0; i < NON_TERMINAL_COUNT; i++){
		grammar->rules[i] = (Rules*)malloc(sizeof(Rules));
		grammar->rules[i]->head = NULL;
		grammar->rules[i]->ruleCount = 0;		
	}

	char ch;

	while(1){
		if(isFileEnd == True)
			return grammar;

		while((ch = fgetc(input))!='<'){
			if(ch == EOF){
				return grammar;
			}
		}
		
		//Load a new rule

		char* nonTerminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE);
		int i = 0;
		while((ch = fgetc(input))!='>'){
			nonTerminalSymbol[i++] = ch;
		}
		nonTerminalSymbol[i] = '\0';

		//Load LHS of the rule -- the corresponding enum
		NonTerminal nonTerm  =  find(nonTerminalSymbol,False);

		//Now look for RHS of the rule
        Rule* newRule = (Rule*)malloc(sizeof(Rule));
        newRule->next = NULL;
        newRule->symbols = NULL;
        newRule->ruleNo = ruleNo;
        ruleNo = ruleNo+1;

        SymbolList* list = (SymbolList*)malloc(sizeof(SymbolList));
        list->head = NULL;
        list->length = 0;

		SymbolNode* currentNode = list->head;

		while(True){
			if(isFileEnd == True)
				break;

			ch = fgetc(input);

			while(ch == ' ' || ch == '\t' || ch == '=' || ch == '>'){
				ch = fgetc(input);
			}

			if(ch == '\n')
				break;

			else if(ch == EOF){
				isFileEnd = True;
				break;
			}

			else if(ch >= 'A' && ch <= 'Z' || ch == 'e'){   // e: epsilon
				
				char* terminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE);
				int i = 0;
				terminalSymbol[i++] = ch;
				ch = fgetc(input);
				while(ch >= 'A' && ch <= 'Z'){
					terminalSymbol[i++] = ch;
					ch = fgetc(input);
				}
				terminalSymbol[i] = '\0';

				SymbolNode* symbolNode = makeSymbolNode(find(terminalSymbol,True), True);

				//Adding Symbol to the list
				currentNode = addToRule(list, symbolNode, currentNode);
				
				if(ch == EOF || ch == '\n'){
					if(ch == EOF)
						isFileEnd = True;
					break;
				}
			}
			else if(ch == '<'){
				char* nonTerminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE);
				int i = 0;
				while((ch = fgetc(input)) != '>'){
					nonTerminalSymbol[i++] = ch;
				}
				nonTerminalSymbol[i] = '\0';
				
				SymbolNode* symbolNode = makeSymbolNode(find(nonTerminalSymbol,False), False);
				//Adding Symbol to the list
				currentNode = addToRule(list, symbolNode, currentNode);			
			}
		}
		
		//Assigning the symbol list to the newRule
		newRule->symbols = list;

		//Add new rule to grammar
        newRule->next = grammar->rules[nonTerm]->head;
        grammar->rules[nonTerm]->head = newRule;
        grammar->rules[nonTerm]->ruleCount++;
        grammar->ruleCount;
	}
} 

SymbolNode* makeSymbolNode(int enum_int, int term_or_nonterm){
	SymbolNode* symbolNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	symbolNode->next=NULL;
	Symbol type;
	if(term_or_nonterm==0){
		type.term = (Token_type)enum_int;
	}
	else{
		type.nonterm = (NonTerminal)enum_int;
	}
	symbolNode->symbol = type;
	symbolNode->isTerminal = term_or_nonterm;

	return symbolNode;
}

int find(char* str, int term_or_nonterm){
	
	//Terminal
	if(term_or_nonterm==0){
		for(int i=0;i<TERMINAL_COUNT;i++){
			if(strcmp(str,terminalMap[i])==0)
				return i;
		}
	}

	//Non terminal
	else{
		for(int i=0;i<NON_TERMINAL_COUNT;i++){
			if(strcmp(str,nonTerminalMap[i])==0)
				return i;
		}
	}
}


SymbolNode* addToRule(SymbolList* list, SymbolNode* symbolNode, SymbolNode* currentNode){
	
	//If initially the list is empty
	if(currentNode==NULL){
		symbolNode->next = list->head;
		list->head = symbolNode;
		currentNode = list->head;
	}
	else{
		currentNode->next = symbolNode;
		currentNode = currentNode->next;
	}

	list->length++;

	return currentNode;
}

void printGrammar(Grammar* grammar){
	printf("\n------------------------ Printing Grammar from Data Structure---------------------\n\n");

	for(int i=0;i<NON_TERMINAL_COUNT;i++){
		printf("%d.  <%s> ===> ",(i+1), nonTerminalMap[i]);

		Rules* rules = grammar->rules[i];

		Rule* temp = rules->head;

		for(int j = 0;j< rules->ruleCount;j++){
			if(j!=0)
				printf("| ");

			SymbolList* symbols = temp->symbols;
			SymbolNode* temp2 = symbols->head;

			for(int k=0;k<symbols->length;k++){
				if(temp2->isTerminal==1)
					printf("%s", terminalMap[temp2->symbol.term]);
				else
					printf("<%s>", nonTerminalMap[temp2->symbol.nonterm]);

				printf(" ");
				temp2 = temp2->next;
			}

			
			temp = temp->next;
		}
		printf("\n");
	}

	printf("\n------------------------ Grammar Over---------------------\n");
}