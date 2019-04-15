/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"
// #include "semanticAnalyzer.h"
#include "lexer.h"
#include "parser.h"
#include "semanticAnalyzer.h"

extern char* terminalMap[];

recordTable* recTable;

STSymbolTable* newSTSymbolTable(int nSlots){
    STSymbolTable* table = (STSymbolTable*) malloc(sizeof(STSymbolTable));
    table->nSlots = nSlots;
    table->slots = (STSlotsList**) malloc(nSlots*sizeof(STSlotsList*));
    table->offset = 0;
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
    symbol->offset = table->offset;
    table->offset = table->offset + symbol->width;
}

void addRecordDef(recordTable* table, recordDef* record){
    int k = getHash(record->name, table->nSlots);
    recordDefNode* node = (recordDefNode*)malloc(sizeof(recordDefNode));
    node->next = table->slots[k];
    node->record = record;
    table->slots[k] = node;
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

recordDef* getRecord(char* name, recordTable* rTable){
    int k = getHash(name, rTable->nSlots);
    recordDefNode* temp = rTable->slots[k];
    while(temp!=NULL){
        if(strcmp(temp->record->name, name) == 0) return temp->record;
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

void traverseASTPostorderForRecords(AST ast){
    if(ast == NULL) return;
    if(ast->leaf == true && ast->lu->token == TK_RECORDID && ast->parent->label == TYPE_DEF){
        recordDef* information = getRecord(ast->lu->lexeme, recTable);
        if(information == NULL){
            recordDef* rec = makeRecordDef(ast->parent);
            addRecordDef(recTable, rec);
        }
        else{
            printf("Error Caught: Record '%s' already defined'\n", ast->lu->lexeme);
        }
        // setParentFn(ast);
    }

    ASTChildren* childs = ast->children;
    ASTNode* symTemp;

    if(childs != NULL){
        symTemp = childs->head;
        while(symTemp != NULL){
            traverseASTPostorderForRecords(symTemp);
            symTemp = symTemp->next;
        }
    }
}

void traverseASTPostorder(AST ast, STTree currentSTTree, int* num, STTree globalTree){
    if(ast == NULL) return;
    ast->currentScope = currentSTTree;
    STTree newSTTree = currentSTTree;
    if(ast->leaf == false && ast->label == MAIN)
        newSTTree = makeSTTreeNode(currentSTTree, "_main"); 
    if(ast->leaf == false && ast->label == FN)
        newSTTree = makeSTTreeNode(currentSTTree, ast->children->head->lu->lexeme); 

    if(ast->leaf == true && ast->lu->token==TK_FUNID && ast->parent->label == FN){
        STSymbolNode* information = getSymbol(ast->lu->lexeme, ast->parent->currentScope->table);
        STSymbolNode* globalInformation = getSymbol(ast->lu->lexeme, globalTree->table);
        if(information == NULL && globalInformation == NULL){
            *num += 1;
            STSymbol* sym = makeSTSymbol(ast, *num);
            addSTSymbol(ast->parent->currentScope->table, ast->lu->lexeme, sym);
        }
        else{
            printf("Error Caught: Function overloading for function '%s()'\n", ast->lu->lexeme);
        }
        setParentFn(ast);
    }
    else if(ast->leaf == true && ast->lu->token==TK_FUNID){
        setParentFn(ast);
        STSymbolNode* information = getSymbol(ast->lu->lexeme, currentSTTree->table);
        STSymbolNode* globalInformation = getSymbol(ast->lu->lexeme, globalTree->table);
        bool recr = checkRecursion(ast);
        if(recr == true){
            ast->token = TK_ERROR; // Something in SemanticAnalyzer perhaps
            printf("Error:Recursive Function Call '%s()' encountered\n", ast->lu->lexeme);
        }

        else if(information == NULL && globalInformation == NULL){
            ast->token = TK_ERROR;
            printf("Error: Undefined Function '%s()' Called\n", ast->lu->lexeme);
        }
    }
    else if(ast->leaf == true && (ast->parent->label == DECLARATION || ast->parent->label == PAR_LIST) && ast->lu->token != TK_INT && ast->lu->token != TK_REAL && ast->lu->token != TK_RECORD){
        STSymbolNode* information = getSymbol(ast->lu->lexeme, currentSTTree->table);
        STSymbolNode* globalInformation = getSymbol(ast->lu->lexeme, globalTree->table);
        if(information != NULL || globalInformation != NULL){
            printf("Error: Multiple Declaration for Variable '%s'\n", ast->lu->lexeme);
        }
        else{
            *num = *num+1;
            STSymbol* sym = makeSTSymbol(ast, *num);
            if(sym->ASTNode->global)
                addSTSymbol(globalTree->table, ast->lu->lexeme, sym);    
            else
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
            printf("Error: Undeclared Variable '%s' Used\n", ast->lu->lexeme);
        }
    }
    ASTChildren* childs = ast->children;
    ASTNode* symTemp;

    if(childs != NULL){
        symTemp = childs->head;
        while(symTemp != NULL){
            traverseASTPostorder(symTemp, newSTTree, num, globalTree);
            symTemp = symTemp->next;
        }
    }
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
    STTree globalTree = NULL;
    char* mainscope = (char*)malloc(sizeof(char)*8);  //can be 6, I guess?
    strcpy(mainscope, "global");
    globalTree = makeSTTreeNode(NULL, mainscope);
    recTable = (recordTable*)malloc(sizeof(recTable));
    recTable->nSlots = 8;
    recTable->slots = (recordDefNode**) malloc(sizeof(recordDefNode*)*recTable->nSlots);
    for(int i=0; i<recTable->nSlots; ++i)
        recTable->slots[i] = NULL;
    int num = 0;
    traverseASTPostorderForRecords(ast);
    traverseASTPostorder(ast, globalTree, &num, globalTree);
    return globalTree;
}

STSymbol* makeSTSymbol(ASTNode* node, int num){
    STSymbol* symbol = (STSymbol*) malloc(sizeof(STSymbol));
    symbol->lu = node->lu; //AST
    symbol->offset = 0;
    symbol->width = 0;
    symbol->number = num;
    symbol->ASTNode = node;
    if(node->parent->children->head->label == RECORD){
        symbol->datatype = TK_RECORD;
    }
    else if(symbol->lu->token!=TK_FUNID){
        symbol->datatype = node->parent->children->head->lu->token; //AST
    }
    else symbol->datatype = TK_FUNID;

    if(symbol->datatype==TK_INT) 
        symbol->width = 2;
    else if(symbol->datatype==TK_REAL) 
        symbol->width = 4;
    else if(symbol->datatype == TK_RECORD){
        recordDef* information = getRecord(node->parent->children->head->children->head->lu->lexeme, recTable);
        symbol->width = information->width;
        symbol->rec = information;
    }
    symbol->isAssigned = false;
    symbol->val = NULL;
    return symbol;
}

recordDef* makeRecordDef(AST ast){
    AST curr = ast;
    if(curr->children->head->next->children->head == NULL || curr->children->head->next->children->head->next == NULL){
        printf("ERROR: Insufficient number of fields in record!\n");
        return NULL;
    }
    recordDef* record = (recordDef*)malloc(sizeof(recordDef));
    record->width = 0;
    record->name = curr->children->head->lu->lexeme;
    record->head = (recordFieldNode*)malloc(sizeof(recordFieldNode));
    recordFieldNode* temp = record->head;
    temp->name = curr->children->head->next->children->head->children->head->next->lu->lexeme;
    temp->type = curr->children->head->next->children->head->children->head->lu->token;
    if(temp->type == TK_INT)
        record->width += 2;
    else if(temp->type == TK_REAL)
        record->width += 4;
    temp->next = (recordFieldNode*)malloc(sizeof(recordFieldNode));
    temp = temp->next;
    temp->name = curr->children->head->next->children->head->next->children->head->next->lu->lexeme;
    temp->type = curr->children->head->next->children->head->next->children->head->lu->token;
    if(temp->type == TK_INT)
        record->width += 2;
    else if(temp->type == TK_REAL)
        record->width += 4;
    temp->next = NULL;
    curr = curr->children->head->next->children->head->next->next;
    while(curr != NULL){
        temp->next = (recordFieldNode*)malloc(sizeof(recordFieldNode));
        temp = temp->next;
        temp->name = curr->children->head->children->head->next->lu->lexeme;
        temp->type = curr->children->head->children->head->lu->token;
        if(temp->type == TK_INT)
            record->width += 2;
        else if(temp->type == TK_REAL)
            record->width += 4;
        temp->next = NULL;
        curr = curr->children->head->next;
    }
    return record;
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
    printf("%20s %20s %20s %20s\n\n","Lexeme","type","scope","offset");
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
        char type[30];
        type[0] = '\0';
        if(symNode->symbol->datatype == TK_INT)
            strcpy(type, "int");
        else if(symNode->symbol->datatype == TK_REAL)
            strcpy(type, "real");
        else if(symNode->symbol->datatype == TK_RECORD){
            recordDef* rec = symNode->symbol->rec;
            recordFieldNode* curr = rec->head;
            while(curr != NULL){
                if(curr->type == TK_INT)
                    strcat(type, "int");
                else if(curr->type == TK_REAL)
                    strcat(type, "real");
                if(curr->next != NULL)
                    strcat(type,"x");
                curr = curr->next;
            }
        }
        else if(symNode->symbol->datatype == TK_FUNID){
            symNode=symNode->next;
            continue;    
        }

        if(symNode->symbol->ASTNode->global){
            printf("%20s %20s %20s %20d\n",symNode->symbol->lu->lexeme,type,"global",symNode->symbol->offset);
        }
        else if (st_par==NULL){
            printf("%20s %20s %20s %20d\n",symNode->symbol->lu->lexeme,type,"None",symNode->symbol->offset);
    
        }
        else{
            printf("%20s %20s %20s %20d\n",symNode->symbol->lu->lexeme,type, sc,symNode->symbol->offset);
    
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

void printGlobalVars(STTree tree){
    printf("\n\n---------------Global Variables Display-------------\n\n");
    printf("%20s %20s %20s\n\n","Variable","type", "offset");
    printGlobalVarsTraversal(tree);
}

void printGlobalVarsTraversal(STTreeNode* node){

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
        char type[30];
        type[0] = '\0';
        if(symNode->symbol->datatype == TK_INT)
            strcpy(type, "int");
        else if(symNode->symbol->datatype == TK_REAL)
            strcpy(type, "real");
        else if(symNode->symbol->datatype == TK_RECORD){
            recordDef* rec = symNode->symbol->rec;
            recordFieldNode* curr = rec->head;
            while(curr != NULL){
                if(curr->type == TK_INT)
                    strcat(type, "int");
                else if(curr->type == TK_REAL)
                    strcat(type, "real");
                if(curr->next != NULL)
                    strcat(type,"x");
                curr = curr->next;
            }
        }
        else if(symNode->symbol->datatype == TK_FUNID){
            symNode=symNode->next;
            continue;    
        }

        if(symNode->symbol->ASTNode->global){
            printf("%20s %20s %20d\n",symNode->symbol->lu->lexeme,type,symNode->symbol->offset);
        }
        
        symNode=symNode->next;
    }
    printf("\n");

    STScopeNest *ch=node->children;

    STTreeNode * ch1;
    if (ch!=NULL){
        ch1=ch->head;

        while (ch1!=NULL){
            printGlobalVarsTraversal(ch1);
            ch1=ch1->next;

        }
    }
}

void printFnMemories(STTree tree){
    printf("\n\n---------------Function Memory Allocation-------------\n\n");
    printf("%20s %20s\n\n","Function","Memory");
    printFnMemoriesTraversal(tree);
    printf("\n\n");
}

void printFnMemoriesTraversal(STTreeNode* node){
    char * st_par;
    char * sc=node->fnscope;
    int sumMem = 0;

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
        if(symNode->symbol->datatype == TK_INT)
            sumMem+=2;
        else if(symNode->symbol->datatype == TK_REAL)
            sumMem+=4;
        else if(symNode->symbol->datatype == TK_RECORD){
            recordDef* rec = symNode->symbol->rec;
            recordFieldNode* curr = rec->head;
            while(curr != NULL){
                if(curr->type == TK_INT)
                    sumMem+=2;
                else if(curr->type == TK_REAL)
                    sumMem+=4;
                curr = curr->next;
            }
        }
        else if(symNode->symbol->datatype == TK_FUNID){
            symNode=symNode->next;
            continue;    
        }
        symNode=symNode->next;
    }

    if(strcmp(sc, "global")!=0 && strcmp(sc, "_main")!=0)
        printf("%20s %20d\n", sc, sumMem);

    STScopeNest *ch=node->children;

    STTreeNode * ch1;
    if (ch!=NULL){
        ch1=ch->head;

        while (ch1!=NULL){
            printFnMemoriesTraversal(ch1);
            ch1=ch1->next;

        }
    }
}

void printTypeExpressionGlobalRecord(STTree tree){
    printf("\n\n----------Global Record Type Expressions & Width---------\n\n");
    printf("%20s %20s %20s\n\n","Name","Type","Width");
    printRecordDefs(tree);
    printf("\n\n");
}

void printRecordDefs(){
    char type[30];
    type[0] = '\0';
    for(int i=0; i< recTable->nSlots; ++i){
        recordDefNode* record = recTable->slots[i];
        if(record == NULL)
            continue;
        recordDef* rec = record->record;
        recordFieldNode* curr = rec->head;
        while(curr != NULL){
            if(curr->type == TK_INT)
                strcat(type, "int");
            else if(curr->type == TK_REAL)
                strcat(type, "real");
            if(curr->next != NULL)
                strcat(type,", ");
            curr = curr->next;
        }
        printf("%20s %20s %20d\n",rec->name,type,rec->width);
    }
}