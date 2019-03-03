#include <string.h>
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
    "TK_ERROR",
    "e",
    "$"
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
    bool alternateRule = false;

	FILE* input = fopen(inputFile, "r");

	if(input == NULL){
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	bool isFileEnd = false;

    Grammar* grammar = (Grammar*)malloc(sizeof(Grammar));
	grammar->ruleCount = 0;
	grammar->rules = (Rules**)malloc(sizeof(Rules*)*NON_TERMINAL_COUNT);
	for(int i = 0; i < NON_TERMINAL_COUNT; i++){
		grammar->rules[i] = (Rules*)malloc(sizeof(Rules));
		grammar->rules[i]->head = NULL;
		grammar->rules[i]->ruleCount = 0;		
	}

	char ch;
    NonTerminal nonTerm;
	while(1){
		if(isFileEnd == true)
			return grammar;

		if(!alternateRule){
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
            // printf("LHS symbol: %s\n",nonTerminalSymbol);
            //Load LHS of the rule -- the corresponding enum
            nonTerm  =  find(nonTerminalSymbol,false);
        }

        alternateRule = false;

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

		while(true){
			if(isFileEnd == true)
				break;

			ch = fgetc(input);
            
			while(ch == ' ' || ch == '\t' || ch == '=' || ch == '>'){
				ch = fgetc(input);
			}
            // printf("\t\tCharacter read: %c\n",ch);
			if(ch == '\n')
				break;

			else if(ch == EOF){
				isFileEnd = true;
				break;
			}

            else if(ch == '|'){
                    alternateRule = true;
                    // printf("\t*** OR ***\n");
                    break;
            }

			else if(ch >= 'A' && ch <= 'Z' || ch == '_' || ch == 'e'){   // e: eps
				char* terminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE);
				int i = 0;
				terminalSymbol[i++] = ch;
				ch = fgetc(input);
				while(ch >= 'A' && ch <= 'Z' || ch == '_'){
					terminalSymbol[i++] = ch;
					ch = fgetc(input);
				}

                if(ch == '|'){
                    alternateRule = true;
                    // printf("\t*** OR ***\n");
                    break;
                }
                
				terminalSymbol[i] = '\0';
				SymbolNode* symbolNode = makeSymbolNode(find(terminalSymbol,true), true);

				//Adding Symbol to the list
				currentNode = addToRule(list, symbolNode, currentNode);
				// printf("\t%s\n",terminalSymbol);
				if(ch == EOF || ch == '\n'){
					if(ch == EOF)
						isFileEnd = true;
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
				
				SymbolNode* symbolNode = makeSymbolNode(find(nonTerminalSymbol,false), false);
				//Adding Symbol to the list
				currentNode = addToRule(list, symbolNode, currentNode);	
                // printf("\t%s\n",nonTerminalSymbol);
			}
		}

		//Assigning the symbol list to the newRule
		newRule->symbols = list;
		//Add new rule to grammar
        newRule->next = grammar->rules[nonTerm]->head;
        grammar->rules[nonTerm]->head = newRule;
        grammar->rules[nonTerm]->ruleCount++;
        grammar->ruleCount++;
        
	}
} 

SymbolNode* makeSymbolNode(int enum_int, int term_or_nonterm){
	SymbolNode* symbolNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	symbolNode->next=NULL;
	Symbol symbol;
	if(term_or_nonterm==0){
		symbol.term = (Token_type)enum_int;
	}
	else{
		symbol.nonterm = (NonTerminal)enum_int;
	}
	symbolNode->symbol = symbol;
	symbolNode->isTerminal = term_or_nonterm;

	return symbolNode;
}

int find(char* str, bool isTerminal){
	
	//Terminal
	if(isTerminal){
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
    printf("\n\nGrammar:\n\n\n");
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
}

FirstAndFollow* getFirstAndFollowSets(Grammar* grammar){

	FirstAndFollow* ffSets = (FirstAndFollow*)malloc(sizeof(FirstAndFollow));

	ffSets->first = (bool**)malloc(NON_TERMINAL_COUNT*sizeof(bool*));
	ffSets->follow = (bool**)malloc(NON_TERMINAL_COUNT*sizeof(bool*));
	
	for(int i=0; i<NON_TERMINAL_COUNT; ++i){
		ffSets->first[i] = (bool*)malloc(TERMINAL_COUNT*sizeof(bool));
		ffSets->follow[i] = (bool*)malloc(TERMINAL_COUNT*sizeof(bool));
		memset(ffSets->first[i],0,sizeof(ffSets->first[i]));
		memset(ffSets->follow[i],0,sizeof(ffSets->follow[i]));		
	}

	for(int i=0; i < NON_TERMINAL_COUNT; ++i){
		computeFirstSet(grammar,(NonTerminal)i, ffSets->first);		
	}

	computeFollowSet(grammar,ffSets);
	return ffSets;
}

void computeFirstSet(Grammar* grammar, NonTerminal nonTerminal, bool** first){
	Rules* rules = grammar->rules[nonTerminal];
	Rule* temp = rules->head;
	for(int j = 0;j< rules->ruleCount;j++){
		SymbolList* symbols = temp->symbols;
		SymbolNode* temp2 = symbols->head;
		int k;
		for(k=0; k < symbols->length; k++){
			//If a terminal has occurred
			if(temp2->isTerminal == true){
				addToSet(first[nonTerminal],temp2->symbol.term);				
				break;
			}
			else{
                if(nonTerminal!=temp2->symbol.nonterm){
                    computeFirstSet(grammar, temp2->symbol.nonterm,first);
				    setUnion(first[nonTerminal], first[temp2->symbol.nonterm]);
                }
				if(!first[temp2->symbol.nonterm][EPS]){
					break;
				}
			}			
			temp2 = temp2->next;
		}
		if(k == symbols->length){
			addToSet(first[nonTerminal],EPS);
		}
		temp = temp->next;
	}
}

void computeFollowSet(Grammar* grammar, FirstAndFollow* sets){
	addToSet(sets->follow[program],DOLLAR);
	bool hasChanged = true;
	while(hasChanged){
		hasChanged = computeFollowUtil(grammar,sets->first,sets->follow);
	}
}

bool computeFollowUtil(Grammar* grammar, bool** first, bool** follow){
    bool hasChanged = false;
	for(int i=0; i<NON_TERMINAL_COUNT; ++i){
		Rules* rules = grammar->rules[i];
		Rule* temp = rules->head;
		for(int j = 0; j< rules->ruleCount; ++j){
			SymbolList* symbols = temp->symbols;
			SymbolNode* temp2 = symbols->head;

			for(int k = 0; k < symbols->length; ++k){
				if(temp2->isTerminal == false){
					SymbolNode* temp3 = temp2->next;
					while(temp3 != NULL){
						if(temp3->isTerminal == true){
							if(!follow[temp2->symbol.nonterm][ temp3->symbol.term]){
								hasChanged = true;
							    addToSet(follow[temp2->symbol.nonterm], temp3->symbol.term);
                            }
							break;
						}
						else{
							hasChanged = hasChanged | setUnion(follow[temp2->symbol.nonterm],first[temp3->symbol.nonterm]);
							if(!first[temp3->symbol.nonterm][EPS]){
								break;
							}
						}
						temp3 = temp3->next;						
					}
					if(temp3 == NULL){
						hasChanged = hasChanged | setUnion(follow[temp2->symbol.nonterm],follow[i]);
					}
				}								
				temp2 = temp2->next;
			}
			temp = temp->next;
		}		
	}
	return hasChanged;
}

void addToSet(bool* set, int index){
    set[index] = true;
}

bool setUnion(bool* a, bool* b){
    bool hasChanged = false;
    for(int i = 0; i < TERMINAL_COUNT; ++i){
        if(i == EPS)
            continue;
        if(a[i] != (a[i]|b[i])){
            hasChanged = true;
            a[i] = a[i] | b[i];
        }
    }
    return hasChanged;
}

void printFirstAndFollow(FirstAndFollow* sets){
	printf("\n\nFirst Set:\n\n");
	for(int i=0; i < NON_TERMINAL_COUNT; ++i){
		printf("%d. %s => ",(i+1),nonTerminalMap[i]);
		printSet(sets->first[i]);
	}	
	printf("\n\nFollow Set:\n\n");
	for(int i=0;i<NON_TERMINAL_COUNT;i++){
		printf("%d. %s => ",(i+1),nonTerminalMap[i]);
		printSet(sets->follow[i]);
	}
}

void printSet(bool* set){
	printf("{ ");
	for(int i=0;i< TERMINAL_COUNT; ++i){
		if(set[i]){
			printf("%s ",terminalMap[i]);
		}
	}
	printf("}\n");
}

void createParseTable(Grammar* grammar, FirstAndFollow* sets, ParsingTable* table){
	for(int i=0;i<NON_TERMINAL_COUNT;i++){
		Rules* rules = grammar->rules[i];

		Rule* temp = rules->head;

		for(int j = 0;j< rules->ruleCount;j++){

			SymbolList* symbols = temp->symbols;
			SymbolNode* temp2 = symbols->head;

			int to_be_continued = 0;

			for(int k=0;k<symbols->length;k++){

				to_be_continued = 0;

				//Terminal
				if(temp2->isTerminal==true){

					//If it 
					if(temp2->symbol.term==EPS){
						for(int x=0;x<TERMINAL_COUNT;x++){
							
							if(sets->follow[i][x]){							
                                Cell* tempaf = get_cell(temp,i);
								if(table->cells[i][x]!=NULL && tempaf->rule!=table->cells[i][x]->rule){
									printf("ERROR: Multiple rules clashing in a table-cell <%s> -> %s\n",nonTerminalMap[i],terminalMap[x]);
								}

								// table->cells[i][x] = get_cell(temp,i);
                                table->cells[i][x] = tempaf;
							}
						}

						continue;
					}

					//any other terminal
					else{
                        Cell* tempaf = get_cell(temp,i);
						if(table->cells[i][temp2->symbol.term]!=NULL && tempaf->rule!=table->cells[i][temp2->symbol.term]->rule){
							printf("ERROR: Multiple rules clashing in a table-cell <%s> -> %s\n",nonTerminalMap[i],terminalMap[temp2->symbol.term]);
						}
						// table->cells[i][temp2->symbol.term] = get_cell(temp,i);
                        table->cells[i][temp2->symbol.term] = tempaf;
						break;
					}
				}

				//Non-terminal
				else{
					for(int x=0;x<TERMINAL_COUNT;x++){

						if(sets->first[temp2->symbol.nonterm][x]){

							if(x!=EPS){
                                Cell* tempaf = get_cell(temp,i);
								if(table->cells[i][x]!=NULL && table->cells[i][x]->rule!=tempaf->rule){
									printf("ERROR: Multiple rules clashing in a table-cell <%s> -> %s\n",nonTerminalMap[i],terminalMap[x]);
								}
								// table->cells[i][x] = get_cell(temp,i);
                                table->cells[i][x] = tempaf;

							}

							// exists, then continue
							else{
								to_be_continued = 1;
							}
						}
					}
				}

				if(to_be_continued==1)
					continue;
				else
					break;

				temp2 = temp2->next;
			}


			temp = temp->next;
		}

		
	}
}

Cell* get_cell(Rule* rule, int non_term_index){
	Cell* new_cell = (Cell*)malloc(sizeof(Cell));
	new_cell->rule = rule;
	new_cell->non_term_index = non_term_index;

	return new_cell;
}

ParsingTable* initialize_Table(){
	ParsingTable* table = (ParsingTable*)malloc(sizeof(ParsingTable));
	for(int i=0;i<NON_TERMINAL_COUNT;i++){
		for(int j=0;j<TERMINAL_COUNT;j++){
			table->cells[i][j] = NULL;
		}
	}

	return table;
}

void print_parsing_table(ParsingTable* table){
	printf("\n\nPrinting Parsing Table:\n\n");
	for(int i=0;i<NON_TERMINAL_COUNT;i++){
		printf("%2d. %25s : ",(i+1),nonTerminalMap[i]);
		for(int z=0;z<TERMINAL_COUNT;z++){
			if(table->cells[i][z]!=NULL){
				printf("1 ");
				//printf("%d ",table->cells[i][z]->rule->rule_no);
			}
			else
				printf("0 ");
			printf(" ");
		}

		printf("\n\n");
	}
}
