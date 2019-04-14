#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"
// #include "semanticAnalyzer.h"
#include "lexer.h"
#include "parser.h"
#include "semanticAnalyzer.h"

extern char* terminalMap[];


STSymbolTable* newSTSymbolTable(int nSlots){
    STSymbolTable* table = (STSymbolTable*) malloc(sizeof(STSymbolTable));
    table->nSlots = nSlots;
    table->slots = (STSlotsList**) malloc(nSlots*sizeof(STSlotsList*));

    for(int i=0; i<nSlots; ++i){
        table->slots[i] = (STSlotsList*) malloc(sizeof(STSlotsList));
        table->slots[i]->head = NULL;
        table->slots[i]->nSymbols = 0;
    }
    return table;
}

int getHash(char* key, int nSlots){
	int sum = 0;
	for(int i=0; i<strlen(key); ++i){
		sum = (sum*7 + ((int)key[i]))%1000000007;
		if(sum<0)
			sum = 0;
	}
	int hashvalue = sum%nSlots;
	return hashvalue;
}

void addSTSymbol(STSymbolTable* table, char* key, STSymbol* symbol){
    int k = getHash(key, table->nSlots);
    STSymbolNode* node = (STSymbolNode*) malloc(sizeof(STSymbolNode));
    node->next = table->slots[k]->head;
    node->symbol = symbol;
    table->slots[k]->nSymbols++;    
    table->slots[k]->head = node;
}

STSymbolNode* getSymbol(char* key, STSymbolTable* lTable){
    int k = getHash(key, lTable->nSlots);
    STSymbolNode* temp = lTable->slots[k]->head;
    while(temp!=NULL){
        if(strcmp(temp->symbol->lu->lexeme,key)==0) return temp;
        temp = temp->next;
    }
    return NULL;
}

STTreeNode* makeSTTreeNode(STTreeNode* parent, char* fnscope){
    STTreeNode* node = (STTreeNode*) malloc(sizeof(STTreeNode));
    node->table = newSTSymbolTable(NO_OF_SLOTS);
    node->children = (STScopeNest*) malloc(sizeof(STScopeNest));
    node->children->head = NULL;
    node->children->nSiblings = 0;
    node->fnscope = fnscope;
    node->parent = parent;
    node->next = NULL;
    if(parent!=NULL){
        node->next = parent->children->head;
        parent->children->head = node;
        parent->children->nSiblings++;
    }
    return node;
}

void traverseASTPostorder(AST ast, STTree currentSTTree, int* num){
    if(ast == NULL) return;
    ast->currentScope = currentSTTree;
    STTree newSTTree = currentSTTree;
    if(ast->leaf == false && ast->label == FN)
        newSTTree = makeSTTreeNode(currentSTTree, ast->children->head->lu->lexeme); 

    if(ast->leaf == true && ast->lu->token==TK_FUNID && ast->parent->label == FN){
        STSymbolNode* information = getSymbol(ast->lu->lexeme, ast->parent->currentScope->table);
        if(information == NULL){
            *num += 1;
            STSymbol* sym = makeSTSymbol(ast, *num);
            addSTSymbol(ast->parent->currentScope->table, ast->lu->lexeme, sym);
        }
        else{
            printf("Error Caught: Function overloading for function '%s()'", ast->lu->lexeme);
        }
        setParentFn(ast);
    }
    else if(ast->leaf == true && ast->lu->token==TK_FUNID){
        setParentFn(ast);
        STSymbolNode* information = getSymbol(ast->lu->lexeme, currentSTTree->table);
        bool recr = checkRecursion(ast);
        if(recr == true){
            ast->token = TK_ERROR; // Something in SemanticAnalyzer perhaps
            printf("Error:Recursive Function Call '%s()' encountered", ast->lu->lexeme);
        }

        else if(information == NULL){
            ast->token = TK_ERROR;
            printf("Error: Undefined Function '%s()' Called", ast->lu->lexeme);
        }
    }

    else if(ast->leaf == true && (ast->parent->label == DECLARATION || ast->parent->label == PAR_LIST) && ast->lu->token != TK_INT && ast->lu->token != TK_REAL && ast->lu->token != TK_RECORD){
        STSymbolNode* information = getSymbol(ast->lu->lexeme, currentSTTree->table);
        if(information != NULL){
            printf("Error: Multiple Declaration for Variable '%s'", ast->lu->lexeme);
        }
        else{
            *num = *num+1;
            STSymbol* sym = makeSTSymbol(ast, *num);
            addSTSymbol(ast->currentScope->table, ast->lu->lexeme, sym);
        }
    }

    else if(ast->leaf == true && ast->lu->token == TK_ID){
        STSymbolNode* information = getInfoFromAST(ast);
        if(information != NULL){
            ast->token = information->symbol->datatype;
        }
        else{
            ast->token = TK_ERROR;
            printf("Error: Undeclared Variable '%s' Used", ast->lu->lexeme);
        }
    }
    ASTChildren* childs = ast->children;
    ASTNode* symTemp;

    if(childs != NULL){
        symTemp = childs->head;
        while(symTemp != NULL){
            traverseASTPostorder(symTemp, newSTTree, num);
            symTemp = symTemp->next;
        }
    }
    // symTemp = ast->extend;
    traverseASTPostorder(symTemp, currentSTTree, num);
}

void setParentFn(AST ast){
    AST temp = ast->parent->parent;
    while(temp!=NULL && temp->label!=FN && temp->label!=PROGRAM)
        temp = temp->parent;
    if(temp->label == PROGRAM)
        ast->callingFunction = NULL;
    else if(temp->label == FN)
        ast->callingFunction = temp->children->head;
}

STTree makeSymbolTables(AST ast){
    STTree tree = NULL;
    char* mainscope = (char*)malloc(sizeof(char)*8);  //can be 6, I guess?
    strcpy(mainscope, "_main");
    tree = makeSTTreeNode(NULL, mainscope);
    int num = 0;
    traverseASTPostorder(ast, tree, &num);
    return tree;
}

STSymbol* makeSTSymbol(ASTNode* node, int num){
    STSymbol* symbol = (STSymbol*) malloc(sizeof(STSymbol));
    symbol->lu = node->lu; //AST
    symbol->offset = 0;
    symbol->width = 0;
    symbol->number = num;
    symbol->ASTNode = node;
    if(symbol->lu->token!=TK_FUNID){
        symbol->datatype = node->parent->children->head->lu->token; //AST
    }
    else symbol->datatype = TK_FUNID;

    if(symbol->datatype==TK_INT) symbol->width = 2;
    else if(symbol->datatype==TK_REAL) symbol->width = 4;
    symbol->isAssigned = false;
    symbol->val = NULL;
    return symbol;
}

STSymbolNode* getInfoFromAST(ASTNode* node){
    STTree tempscope = node->currentScope;
    STSymbolNode* information = NULL;
    while(tempscope != NULL){
        information = getSymbol(node->lu->lexeme, tempscope->table);
        if(information == NULL){
            tempscope = tempscope->parent;
        }
        else{
            break;
        }
    }
    return information;
}

bool checkRecursion(ASTNode* node){
    if(node->callingFunction == NULL){
        return false;
    }
    if(strcmp(node->lu->lexeme, node->callingFunction->lu->lexeme) == 0){
        return true;
    }
    return false;
}

void displaySTTree(STTree tree){
    printf("\n\n---------------Symbol Table Display-------------\n\n");
    printf("%20s %20s %20s %20s","Lexeme","type","scope","offset");
    displaySTTreeTraversal(tree);
}

void displaySTTreeTraversal(STTreeNode* node){

    char * st_par;
    char * sc=node->fnscope;

    if (node->parent==NULL) st_par=NULL;
    else    st_par=node->parent->fnscope;

    STSymbolTable * table=node->table;
    STSlotsList * slots=(STSlotsList *)malloc(sizeof(STSlotsList));
    slots->head=NULL;

    STSymbolNode * symNode;

    for (int i=0;i<NO_OF_SLOTS;++i){
        STSymbolNode * s=table->slots[i]->head;
        while (s!=NULL){
            symNode=(STSymbolNode*)malloc(sizeof(STSymbolNode));
            symNode->symbol=s->symbol;
            symNode->next=slots->head;
            slots->head=symNode;
            
            s=s->next;
        }   
    }
    
    symNode=slots->head;

    while (symNode!=NULL){
        if (st_par==NULL){
            printf("%20s %20s %20s %20d",symNode->symbol->lu->lexeme,terminalMap[symNode->symbol->datatype],"None",symNode->symbol->offset);
    
        }
        else{
            printf("%20s %20s %20s %20d",symNode->symbol->lu->lexeme,terminalMap[symNode->symbol->datatype], sc,symNode->symbol->offset);
    
        }
        symNode=symNode->next;
    }
    printf("\n");

    STScopeNest *ch=node->children;

    STTreeNode * ch1;
    if (ch!=NULL){
        ch1=ch->head;

        while (ch1!=NULL){
            displaySTTreeTraversal(ch1);
            ch1=ch1->next;

        }
    }
}