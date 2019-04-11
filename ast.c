#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "lookup.h"
#include "NaryTree.h"
#include "stack.h"

extern char* terminalMap[];
extern char* nonTerminalMap[];

char* astNodeNameMap[] = {
    "PROGRAM",
    "MAIN",
    "OTHER_FN",
    "FN",
    "PAR_LIST",
    "STMTS",
    "TYPE_DEFS",
    "TYPE_DEF",
    "FIELD_DEFS",
    "FIELD_DEF",
    "MORE_FIELDS",
    "DECLARATIONS",
    "DECLARATION",
    "OTHER_STMTS",
    "ASSIGNMENT_STMT",
    "SINGLE_OR_REC_ID",
    "FUN_CALL_STMT",
    "ITERATIVE_STMT",
    "CONDITIONAL_STMT",
    "ELSE",
    "IO_STMT_READ",
    "IO_STMT_WRITE",
    "ALL_VAR",
    "ARITHMETIC",
    "EXP_PRIME",
    "TERM",
    "TERM_PRIME",
    "ALL",
    "BOOLEAN1",
    "BOOLEAN2",
    "BOOLEAN3",
    "RETURN",
    "ID_LIST"
};

AST makeAST(ParseTree pt){
    if (pt==NULL)   return NULL;

    makeAST_postOrderTraversal(pt);

    AST ast=(AST)malloc(sizeof(ASTNode));
    ast=pt->addr;

}

void makeAST_postOrderTraversal(ParseTree pt)
{
    if (pt==NULL)   return NULL;

    Children * ch= pt->children;

    if(ch!=NULL){

        ParseTree pt1=ch->head;
        while (pt1!=NULL)
        {
            makeAST_postOrderTraversal(pt1);
            pt1=pt1->next;
        }
    }

    if (pt->ruleNo==0)  return;

    makeAST_nodeTraversal(pt);
}

ASTChildren* initializeASTChildren(){
    ASTChildren* children = (ASTChildren*)malloc(sizeof(ASTChildren));
    children->noSiblings = 0;
    children->head = NULL;
    children->tail = NULL;
    return children;
}

void addASTChildren(ASTChildren *ch,AST * ast){
    if (ch->head!=NULL)
    {
        ch->tail->next=ast;
        ch->tail=ch->tail->next;
        
    }
    else
    {
        ch->head=ast;
        ch->tail=ast;
    }
    ch->noSiblings+=1;
}
    
ASTNode* makeASTNode(astNodeName label, bool leaf, ASTNode* parent, ASTChildren* children, ASTNode* next, Lexical_Unit* lu){
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->label = label;
    node->leaf = leaf;
    node->parent = parent;
    node->children = children;
    node->next = next;
    node->lu = lu;

    node->currentScope = NULL;
    node->token = -1;
    node->returnTypes = NULL;

    if(node->leaf)
        node->lineNo = node->lu->line_no;

    node->value = NULL;

    return node;
}

inline ASTNode* makeASTLeaf(Lexical_Unit* lu){
    return makeASTNode(-1,true,(ASTNode*)NULL,(ASTChildren*)NULL,(ASTNode*)NULL,lu);
}

void addParentPointers(ASTNode* parent, ASTChildren* children){
    ASTNode* temp1 = children->head;
    ASTNode* temp2;

    while(temp1 != NULL){
        temp1->parent = parent;
        temp2 = temp1->extend;
        while(temp2!=NULL){
            temp2->parent = parent;
            temp2 = temp2->extend;
        }
        temp1 = temp1->next;
    }
    parent->lineNo = children->head->lineNo;
}

typeList * initializeTypeList()
{
    typeList * tl=(typeList*)malloc(sizeof(typeList));
    tl->head=NULL;
    tl->tail=NULL;
    tl->noSiblings=0; 
}

void addReturnType(typeList *tl, Token_type type)
{
    typeListNode* new=(typeListNode*)malloc(sizeof(typeListNode));
    new->next=NULL;
    new->type=type;

    if (tl->head!=NULL)
    {
        tl->tail->next=new;
        tl->tail=tl->tail->next;
        
    }
    else
    {
        tl->head=new;
        tl->tail=new;
    }
    tl->noSiblings+=1;

}