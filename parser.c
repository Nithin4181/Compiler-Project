/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#include <string.h>
#include "parser.h"
#include "lexer.h"

char* terminalMap[] = { // Maps enum values to corresponding terminal/token names.
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

char* nonTerminalMap[] = {  // Maps enum values to corresponding non-terminal names.
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
    /* Description: Loads grammar from txt file */
    /* Arguments: File name of grammar file */
    /* Return Type: Pointer to grammar structure */
	int ruleNo = 1;
    bool alternateRule = false;

	FILE* inputFile = fopen(inputFile, "r");

	if(inputFile == NULL){						// Handler if an error is thrown while opening a file
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	bool isFileEnd = false;

    Grammar* grammar = (Grammar*)malloc(sizeof(Grammar));  				// Allocate memory to hold all the grammar rules
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
            while((ch = fgetc(inputFile))!='<'){
                if(ch == EOF){
                    return grammar;
		    	}
		    }
            char* nonTerminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE); //Load a new rule from then input file
            int i = 0;
            while((ch = fgetc(inputFile))!='>'){
                nonTerminalSymbol[i++] = ch;
            }
            nonTerminalSymbol[i] = '\0';
            nonTerm  =  find(nonTerminalSymbol,false); //Loading the LHS of the rule
        }
        alternateRule = false;

		//Look for RHS of the rule
        Rule* newRule = (Rule*)malloc(sizeof(Rule));
        newRule->ruleNo = ruleNo;
        ruleNo = ruleNo+1;
        newRule->next = NULL;
        newRule->symbols = NULL;
        
        SymbolList* list = (SymbolList*)malloc(sizeof(SymbolList));
        list->length = 0;
        list->head = NULL;

		SymbolNode* currentNode = list->head;

		while(1){
			if(isFileEnd == true)
				break;

			ch = fgetc(inputFile);
            
			while(ch == ' ' || ch == '\t' || ch == '=' || ch == '>'){
				ch = fgetc(inputFile);
			}
			if(ch == '\n')
				break;

            else if(ch == '|'){
                    alternateRule = true;
                    break;
            }
			else if(ch == EOF){
				isFileEnd = true;
				break;
			}
			// If it's a non-terminal
			else if(ch == '<'){
				char* nonTerminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE);
				int i = 0;
				while((ch = fgetc(inputFile)) != '>'){
					nonTerminalSymbol[i++] = ch;
				}
				nonTerminalSymbol[i] = '\0';
				
				SymbolNode* symbolNode = makeSymbolNode(find(nonTerminalSymbol,false), false);
				currentNode = addToRule(list, symbolNode, currentNode);	 // Add the created symbol to the list
			}
            // If it's a terminal
			else if(ch == 'e' || ch >= 'A' && ch <= 'Z' || ch == '_'){   // e: eps
				char* terminalSymbol = (char*)malloc(sizeof(char)*SYMBOL_SIZE);
				int i = 0;
				terminalSymbol[i++] = ch;
				ch = fgetc(inputFile);
				while(ch == '_' || ch >= 'A' && ch <= 'Z'){
					terminalSymbol[i++] = ch;
					ch = fgetc(inputFile);
				}
                terminalSymbol[i] = '\0';
				SymbolNode* symbolNode = makeSymbolNode(find(terminalSymbol,true), true);
				currentNode = addToRule(list, symbolNode, currentNode); // Add the Symbol created to the existing list
				if(ch == '\n' || ch == EOF){
					if(ch == EOF)
						isFileEnd = true;
					break;
				}
                if(ch == '|'){
                    alternateRule = true;
                    break;
                }
			}
		}
		newRule->symbols = list;
        newRule->next = grammar->rules[nonTerm]->head;
        grammar->rules[nonTerm]->head = newRule;
        grammar->rules[nonTerm]->ruleCount++;
        grammar->ruleCount++;
        // Added the newly created rule to the Grammar
	}
} 

SymbolNode* makeSymbolNode(int index, bool isTerminal){
    /* Description: Make linked list node from terminal/non-terminal details */
    /* Arguments: Enum index of terminal/non-terminal, boolean specifiying if terminal or non-terminal */
    /* Return Type: Pointer to linked list node */    
	SymbolNode* symbolNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	Symbol symbol;
	if(isTerminal==1){
		symbol.nonterm = (NonTerminal)index;
	}
	else{
		symbol.term = (Token_type)index;
	}
	symbolNode->symbol = symbol;
	symbolNode->isTerminal = isTerminal;
	symbolNode->next=NULL;
	return symbolNode;
}

int find(char* str, bool isTerminal){
    /* Description: Get enum index from terminal/non-terminal map */
    /* Arguments: Terminal/non-terminal string and boolean specifying whether terminal or non-terminal */
    /* Return Type: Enum value */    	
	if(isTerminal == 0){
		for(int i=0;i<NON_TERMINAL_COUNT;i++){
			if(strcmp(str,nonTerminalMap[i])==0)
				return i;
		}
	}
	else{
		for(int i=0;i<TERMINAL_COUNT;i++){
			if(strcmp(str,terminalMap[i])==0)
				return i;
		}
	}
}


SymbolNode* addToRule(SymbolList* list, SymbolNode* symbolNode, SymbolNode* currentNode){
    /* Description: Adds terminal/non-terminal to the rule linked list */
    /* Arguments: Rule linked list, node to be added, current node */
    /* Return Type: Updated current node pointer */    	    
	if(currentNode != NULL){
		currentNode->next = symbolNode;
		currentNode = currentNode->next;
	}
	else{								//  List is empty
		symbolNode->next = list->head;
		list->head = symbolNode;
		currentNode = list->head;
	}
	list->length++;
	return currentNode;
}

void printGrammar(Grammar* grammar){
    /* Description: Prints the grammar stored in the grammar data structure */
    /* Arguments: Grammar data structure */
    /* Return Type: Void */    	    
    printf("\n\nGrammar:\n\n\n\n");
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
    /* Description: Compute first and follow sets for the given grammar */
    /* Arguments: Grammar stored in grammar structure */
    /* Return Type: Pointer to first and follow set structure */    	    
	FirstAndFollow* ffSets = (FirstAndFollow*)malloc(sizeof(FirstAndFollow));

	ffSets->first = (bool**)malloc(NON_TERMINAL_COUNT*sizeof(bool*));
	ffSets->follow = (bool**)malloc(NON_TERMINAL_COUNT*sizeof(bool*));
	
	for(int i=0; i<NON_TERMINAL_COUNT; ++i){
		ffSets->first[i] = (bool*)malloc(TERMINAL_COUNT*sizeof(bool));
		ffSets->follow[i] = (bool*)malloc(TERMINAL_COUNT*sizeof(bool));
		for(int j=0; j<TERMINAL_COUNT; ++j){
            ffSets->first[i][j] = false;
            ffSets->follow[i][j] = false;
        }
	}

	for(int i=0; i < NON_TERMINAL_COUNT; ++i){
		computeFirstSet(grammar,(NonTerminal)i, ffSets->first);		
	}

	computeFollowSet(grammar,ffSets);
	return ffSets;
}

void computeFirstSet(Grammar* grammar, NonTerminal nonTerminal, bool** first){
    /* Description: Computes for given non-terminal from given grammar */
    /* Arguments: Grammar, non-terminal, and first set table */
    /* Return Type: void */    	        
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
    /* Description: Compute follow set for given grammar */
    /* Arguments: Grammar and first and follow set structure */
    /* Return Type: void */    	        
	addToSet(sets->follow[program],DOLLAR);
	bool hasChanged = true;
	while(hasChanged){          // Repeat until follow set remains unchanged
		hasChanged = computeFollowUtil(grammar,sets->first,sets->follow);
	}
}

bool computeFollowUtil(Grammar* grammar, bool** first, bool** follow){
    /* Description: Do one pass for follow set computation */
    /* Arguments: Grammar, first set table, and follow set table */
    /* Return Type: boolean specifying if follow set changed in this pass */
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
    /* Description: Add terminal at index in the enum to the set */
    /* Arguments: The set (one row of first/follow set table) and index of terminal */
    /* Return Type: void */    	        
    set[index] = true;
}

bool setUnion(bool* a, bool* b){
    /* Description: Union of two sets */
    /* Arguments: Two sets */
    /* Return Type: boolean specifying if the first set changed */
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
    /* Description: Print first and follow sets */
    /* Arguments: First and follow set structure */
    /* Return Type: void */    	        
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
    /* Description: Print set */
    /* Arguments: Set to be printed */
    /* Return Type: void */
	printf("{ ");
	for(int i=0;i< TERMINAL_COUNT; ++i){
		if(set[i]){
			printf("%s ",terminalMap[i]);
		}
	}
	printf("}\n");
}

ParsingTable makeNewParseTable(){
    /* Description: Make new empty parse table */
    /* Arguments: void */
    /* Return Type: ParsingTable */    
    ParsingTable pt = (ParsingTable)malloc(sizeof(Rule**)*NON_TERMINAL_COUNT);  // PasingTable is a 2D array of pointers to grammar rules
    for (int i = 0; i < NON_TERMINAL_COUNT; ++i) 
         pt[i] = (Rule**)malloc(sizeof(Rule*)*TERMINAL_COUNT); 
    for(int i = 0; i < NON_TERMINAL_COUNT; ++i)
        for(int j = 0; j < TERMINAL_COUNT; ++j)
            pt[i][j] = NULL;
    return pt;
}

void createParseTable(Grammar* grammar, FirstAndFollow* sets, ParsingTable table){
    /* Description: Construct parse table from grammar and first and follow sets */
    /* Arguments: Grammar, first and follow set structure, and parsing table */
    /* Return Type: void */    
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
    /* Description: Prints parsing table */
    /* Arguments: Parsing table */
    /* Return Type: void */    
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
    /* Description: Parse source code from file using parse table and first-follow sets and construct parse tree */
    /* Arguments: Source code file name, parsing table, first and follow sets, and integer to detect parse errors */
    /* Return Type: Parse tree */    

    printf("\n");
	FILE* fp = lexer_initialisation(testcaseFile);  	//Initialize_lexer
	if(fp == NULL){
		printf("\nFile not opened for parsing\n");
		return NULL;
	}
	//Making Start symbol as the root of parse tree
    ParseTree tree = makeNode(false,program,NULL);

	//Initializing Stack
	Stack* stack = initializeStack();

	//Pushing $ and starting nonterminal on the stack
	TreeNode* bottomMarker =  makeNode(true,DOLLAR,NULL);

	push(stack,bottomMarker);
	push(stack,tree);

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
				printf("\n\n########## Source code is syntactically CORRECT ##########\n\n");
			}
			else{
				*parseErrors = 1;
				printf("\n\n########## Source code is syntactically WRONG ##########\n\n");
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
			printf("\n\n########## Source code is syntactically WRONG ##########\n\n");
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
    /* Description: Print parse tree to file */
    /* Arguments: Parse tree and output file name */
    /* Return Type: void */        
	FILE* fp1 = fopen(fname,"w");
	fprintf(fp1,"\n######################################### Parse Tree #########################################\n\n");
	fprintf(fp1,"%-25s %-10s %-15s %-15s %-30s %-5s %s\n\n\n", "Lexeme","LineNo","TokenName","ValueIfNumber","ParentNodeSymbol","IsLeafNode","NodeSymbol");

	printParseTree_util(tree,&fp1);

	fprintf(fp1,"\n######################################### End Of Parse Tree #########################################\n\n");
    fclose(fp1);
}


void printParseTree_util(TreeNode* node, FILE** fp1){
    /* Description: Utility function for printing parse tree to file */
    /* Arguments: Node of parse tree, file pointer to output file */
    /* Return Type: void */        
	if(node == NULL){
		return;
	}
	char* empty = "----";
	char* no = "No";
	char* yes = "Yes";
	char* root = "Root";

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

void printNode(FILE** fp1, TreeNode* node, bool isLeaf, char* empty, char* no, char* yes, char* root){
    /* Description: Print information about one node of parse tree into file */
    /* Arguments: Output file pointer, node of tree, details of node */
    /* Return Type: void */    
	char* error = "<Error>" ;
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