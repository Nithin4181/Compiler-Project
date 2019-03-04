#include <string.h>
#include "parser.h"
#include "lexer.h"

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
			if(ch == '\n')
				break;

			else if(ch == EOF){
				isFileEnd = true;
				break;
			}

            else if(ch == '|'){
                    alternateRule = true;
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
                terminalSymbol[i] = '\0';
                
				// terminalSymbol[i] = '\0';
				SymbolNode* symbolNode = makeSymbolNode(find(terminalSymbol,true), true);

				//Adding Symbol to the list
				currentNode = addToRule(list, symbolNode, currentNode);
				if(ch == EOF || ch == '\n'){
					if(ch == EOF)
						isFileEnd = true;
					break;
				}
                if(ch == '|'){
                    alternateRule = true;
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

ParsingTable makeNewParseTable(){
    ParsingTable pt = (ParsingTable)malloc(sizeof(Rule**)*NON_TERMINAL_COUNT);  // PasingTable is a 2D array of pointers to grammar rules
    for (int i = 0; i < NON_TERMINAL_COUNT; ++i) 
         pt[i] = (Rule**)malloc(sizeof(Rule*)*TERMINAL_COUNT); 
    for(int i = 0; i < NON_TERMINAL_COUNT; ++i)
        for(int j = 0; j < TERMINAL_COUNT; ++j)
            pt[i][j] = NULL;
    return pt;
}

void createParseTable(Grammar* grammar, FirstAndFollow* sets, ParsingTable table){
	for(int i = 0; i < NON_TERMINAL_COUNT; ++i){

		Rules* rules = grammar->rules[i];
		Rule* temp = rules->head;
		
        for(int j = 0;j< rules->ruleCount;j++){

			SymbolList* symbols = temp->symbols;
			SymbolNode* temp2 = symbols->head;

			bool isContinued = false;

			for(int k = 0; k < symbols->length; ++k){
				
                isContinued = false;

				if(temp2->isTerminal){      // Terminal symbol
					
                    if(temp2->symbol.term == EPS){

						for(int x = 0; x < TERMINAL_COUNT; ++x){
							
							if(sets->follow[i][x]){							
								if(table[i][x]!=NULL && temp!=table[i][x]){
									printf("ERROR: More than one rule indexed into the parse table at %s : %s\n",nonTerminalMap[i],terminalMap[x]);
								}
                                table[i][x] = temp;
							}
						}
						continue;
					}
					else{       // Terminal symbol other than eps
						if(table[i][temp2->symbol.term]!=NULL && temp!=table[i][temp2->symbol.term]){
							printf("ERROR: More than one rule indexed into the parse table at %s : %s\n",nonTerminalMap[i],terminalMap[temp2->symbol.term]);
						}
                        table[i][temp2->symbol.term] = temp;
						break;
					}
				}
				else{           // Non-terminal
					for(int x=0;x<TERMINAL_COUNT;x++){
						if(sets->first[temp2->symbol.nonterm][x]){
							if(x!=EPS){
								if(table[i][x]!=NULL && table[i][x]!=temp){
									printf("ERROR: More than one rule indexed into the parse table at %s : %s\n",nonTerminalMap[i],terminalMap[x]);
								}
                                table[i][x] = temp;

							}
							else{
								isContinued = true;
							}
						}
					}
				}
                temp2 = temp2->next;                
				if(isContinued)
					continue;
				else
					break;
			}
			temp = temp->next;
		}
	}
}

void printParsingTable(ParsingTable table){
	printf("\n\nPrinting Parsing Table:\n\n");
	for(int i=0;i<NON_TERMINAL_COUNT;i++){
		printf("%2d. %25s : ",(i+1),nonTerminalMap[i]);
		for(int z=0;z<TERMINAL_COUNT;z++){
			if(table[i][z]!=NULL){
				printf("1 ");
			}
			else
				printf("0 ");
			printf(" ");
		}

		printf("\n\n");
	}
}

ParseTree  parseInputSourceCode(char *testcaseFile, ParsingTable table, FirstAndFollow* ff, int* parseErrors){
	//Initialize_lexer
	FILE* fp = lexer_initialisation(testcaseFile);

	if(fp == NULL){
		printf("\nFile not opened for parsing\n");
		return NULL;
	}

	printf("\n\n------------------------------LEXER INITIALIZED--------------------------\n\n");

	//Making Start symbol as the root of parse tree
    ParseTree tree = makeNode(false,program,NULL);
	printf("\n\n------------------------------TREE INITIALIZED---------------------------\n\n");

	//Initializing Stack
	Stack* stack = initializeStack();

	//Pushing $ and starting nonterminal on the stack
	TreeNode* bottomMarker =  makeNode(true,DOLLAR,NULL);

	push(stack,bottomMarker);
	push(stack,tree);

	printf("\n\n------------------------------STACK INITIALIZED--------------------------\n\n");

	printf("\n\nStart obtaining tokens from lexer and parse them\n\n");

	Token_type lookahead;

	//Initially get a token from the file
	Lexical_Unit* lu = getNextToken(&fp);

	//If first token received is NULL
	if(lu == NULL){
		printf("\n\nInput File Empty\n\n");
		return NULL;			
	}
	
	bool hasError = false;

	while(true){
		//If input is consumed
		if(lu == NULL){

			//If input consumed
			if(bottomOfStack(stack) && !hasError){
				printf("\n\n---------------------STACK EMPTY AND INPUT CONSUMED------------------------\n\n");
				printf("\n\n------Input source code is syntactically CORRECT - PARSING SUCCESSFUL-----------------\n\n");
			}
			else{
				*parseErrors = 1;
				printf("\n\n------Input source code is syntactically WRONG - PARSING NOT SUCCESSFUL---------------\n\n");
			}
			break;
		}

		//Otherwise get the lookahead symbol
		lookahead = lu->token;				

		if(lookahead == TK_ERROR){
			//Skip Lexical Error
            hasError = true;
			*parseErrors=1;
			lu = getNextToken(&fp);
			continue;
		}
        else if(lookahead == TK_COMMENT){
            while(lookahead == TK_COMMENT){
                lu = getNextToken(&fp);
                if(lu == NULL)
                    continue;
                else
                    lookahead = lu->token;
            }
        }

		//If top of the stack is $ and input is still left
		if(bottomOfStack(stack)){
			*parseErrors = 1;
			printf("\n\nUnexpected tokens at the end - Stack empty but program still left\n\n");
			printf("\n\n------Input source code is syntactically WRONG - PARSING NOT SUCCESSFUL---------------\n\n");
			break;
		}

		//Pop the symbol on the top of the stack
		StackNode* stackNode = pop(stack);
		TreeNode* treeNode = stackNode->parseTreeNode;
		//Terminal

		if(treeNode->content->isTerminal){
    
			//Symbol on the stack matches with the lookahead symbol
			if(lookahead == treeNode->content->type.term){
				//Will help in printing the parse tree
				treeNode->lu = lu;
				//Get next Lookahead Symbol
				lu = getNextToken(&fp);
				continue;			
			}
			//If lookahead and top of stack don't match
			else{		
				//Report Error
				hasError = true;
				*parseErrors = 1;                
				fprintf(stderr,"Line %d\t| Syntax error:  The token  for lexeme %s does not match at line %d\n",lu->line_no,lu->lexeme,lu->line_no);

				//Skip lookahead symbols until something matches
				while(lu!=NULL){
					lookahead = lu->token;
					//Check for the current one 
					if(lookahead == treeNode->content->type.term){
						push(stack, treeNode);
						break;
					}					
					//Check for the following one
					else{
                      Rule* ruleToBeUsed = table[treeNode->content->type.nonterm][lookahead];
						if(ruleToBeUsed!=NULL){
							break;
						}
					}
					
					
					if(lu->token == TK_SEM){
						lu = getNextToken(&fp);
						break;
					}
					lu = getNextToken(&fp);
				}
				continue;			
			}	
		}			
		//if top of the stack is a non terminal
		else{
    
			//Get the rule to be applied from the parsing table
            Rule* ruleToBeUsed = table[treeNode->content->type.nonterm][lookahead];

			//If no rule matches
            bool recovered = false;
            if(ruleToBeUsed == NULL){
                // printf("In non terminal error recovery\n");
                *parseErrors = 1;
				fprintf(stderr,"Line %d\t| Syntax error:  The token  for lexeme %s does not match at line %d\n",lu->line_no,lu->lexeme,lu->line_no);
                StackNode* recStackNode = top(stack);
				TreeNode* recNode = recStackNode->parseTreeNode;
                while(lu != NULL){
                    lookahead = lu->token;
                    if(ff->first[recNode->content->type.nonterm][lookahead]){
                        recovered = true;
                        push(stack,recNode);
                        break;
                    }
                    else if(ff->follow[recNode->content->type.nonterm][lookahead]){
                        recovered = true;
                        break;
                    }
                    else
                        lu = getNextToken(&fp);
                }
                if(recovered)
                    continue;
            }
			//Normal Case
			else{		
				//Add Children to the parse tree for the popped non terminal from the stack
				addChildren(treeNode, ruleToBeUsed);
				//PUSH RHS of the rule on the top of the stack
				pushChildrenToStack(stack,treeNode);
			}		
		}
		//Go to begin of the while loop
	}
	if(fp!=NULL)
		fclose(fp);
	return tree;
}


void printParseTree(ParseTree tree, char* fname){
	FILE* fp1 = fopen(fname,"w");
	fprintf(fp1,"\n------------------------------------------------------Printing Parse Tree On the Console-----------------------------------------------\n\n");
	fprintf(fp1,"%-25s %-10s %-15s %-15s %-30s %-5s %s\n\n\n", "LEXEME","LINE","TOKEN","VALUE","PARENT","LEAF","NODE");

	printParseTree_util(tree,&fp1);

	fprintf(fp1,"\n------------------------------------------------------Parse Tree Printed On the Console-----------------------------------------------\n\n");
    fclose(fp1);
}


void printParseTree_util(TreeNode* node, FILE** fp1){
	if(node == NULL){
		return;
	}
	char* empty = "****";
	char* no = "no";
	char* yes = "yes";
	char* root = "ROOT";

	Children* children = node->children;

	//If not a leaf node
	if(children != NULL){
		TreeNode* temp = children->head;
		
		//Printing the left most child
		printParseTree_util(temp,fp1);

		//Then print the node
		printNode(fp1,node, 0, empty,no,yes,root);

		//Then proceed to rest of the children
		temp = temp->next;
		
		while(temp!=NULL){
			printParseTree_util(temp, fp1);
			temp = temp->next;
		}	
	}
	//If Leaf Node, print it
	else{
		printNode(fp1,node, 1, empty,no,yes,root);		
	}
}

//This function prints the stored info at one node of a tree
void printNode(FILE** fp1, TreeNode* node, bool isLeaf, char* empty, char* no, char* yes, char* root){
	char* error = "(ERROR)" ;
	//Leaf Node
	if(isLeaf){	
		//If leaf node is a non-terminal (in-case of incomplete trees) -- ERROR CASE
		if(node->content->isTerminal == false){
			//Root Node
			if(node->parent == NULL){
				fprintf(*fp1,"%-25s %-10s %-15s %-15s %-30s %-5s <%s>\n", empty,empty,empty,empty,root,yes,nonTerminalMap[node->content->type.nonterm]);
			}
			else{
				fprintf(*fp1,"%-25s %-10s %-15s %-15s %-30s %-5s <%s>\n", empty,empty,empty,empty,nonTerminalMap[node->parent->content->type.nonterm],yes,nonTerminalMap[node->content->type.nonterm]);
			}
		}	
		//No lexeme for epsilon
		else if(node->content->type.term==EPS){			
			fprintf(*fp1,"%-25s %-10s %-15s %-15s %-30s %-5s %s\n", empty,empty,empty,empty,nonTerminalMap[node->parent->content->type.nonterm],yes,terminalMap[node->content->type.term]);
		}
		//Some terminal nodes may not be assigned lexical tokens since they are not matched -- ERROR CASE
		else if(node->lu == NULL){
			fprintf(*fp1,"%-25s %-10s %-15s %-15s %-30s %-5s %s\n", error, error, error, empty, nonTerminalMap[node->parent->content->type.nonterm], yes, terminalMap[node->content->type.term]);
		}		
        //Integer
		else if(node->lu->token == TK_NUM){
			fprintf(*fp1,"%-25s %-10d %-15s %-15d %-30s %-5s %s\n", node->lu->lexeme, node->lu->line_no, terminalMap[node->lu->token], node->lu->val->integer, nonTerminalMap[node->parent->content->type.nonterm], yes, terminalMap[node->content->type.term]);
		}
		//Real
		else if(node->lu->token == TK_RNUM){
			fprintf(*fp1,"%-25s %-10d %-15s %-15f %-30s %-5s %s\n", node->lu->lexeme, node->lu->line_no, terminalMap[node->lu->token], node->lu->val->real, nonTerminalMap[node->parent->content->type.nonterm], yes, terminalMap[node->content->type.term]);
		}
		//Not an integer or Real Number
		else{
			fprintf(*fp1,"%-25s %-10d %-15s %-15s %-30s %-5s %s\n", node->lu->lexeme, node->lu->line_no, terminalMap[node->lu->token], empty, nonTerminalMap[node->parent->content->type.nonterm], yes, terminalMap[node->content->type.term]);
		}
	}
	//Non Leaf Node
	else{
		//Root Node
		if(node->parent == NULL){
			fprintf(*fp1,"%-25s %-10s %-15s %-15s %-30s %-5s <%s>\n", empty,empty,empty,empty,root,no,nonTerminalMap[node->content->type.nonterm]);
		}
		else{
			fprintf(*fp1,"%-25s %-10s %-15s %-15s %-30s %-5s <%s>\n", empty,empty,empty,empty,nonTerminalMap[node->parent->content->type.nonterm],no,nonTerminalMap[node->content->type.nonterm]);
		}
	}
}



/************************************************* EXTRA FUNCTIONS (Used for Debugging during parsing) ****************************************************/

void printTopOfStack(TreeNode* treeNode){
	printf("TOP OF STACK: ");
	if(treeNode->content->isTerminal){
		printf("%s\n", terminalMap[treeNode->content->type.term]);
		
	}
	else{
		printf("<%s>\n", nonTerminalMap[treeNode->content->type.nonterm]);
	}
}


void printChildren(Children* children){
	TreeNode* temp = children->head;
	while(temp!=NULL){
		if(temp->content->isTerminal)
			printf("%s ",terminalMap[temp->content->type.term]);
		else{
			printf("<%s> ",nonTerminalMap[temp->content->type.nonterm]);
		}
		temp = temp->next;
	}
}