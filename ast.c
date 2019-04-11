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

void makeAST_nodeTraversal(ParseTree pt){
    int ruleNo=pt->ruleNo;

    switch(ruleNo){
        case  1  :

            break;

        case  2  :

            break;

        case  3  :

            break;

        case  4  :
            pt->addr=NULL;
            break;

        case  5  :

            break;

        case  6  :
            pt->addr=pt->children->head->addr;
            break;

        case  7  :
            pt->addr=pt->children->head->addr;
            break;

        case  8  :
            pt->addr=NULL;
            break;

        case  9  :

            break;

        case  10  :
            pt->addr=pt->children->head->addr;
            break;

        case  11  :
            pt->addr=pt->children->head->addr;
            break;

        case  12  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  13  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  14  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  15  :
            pt->addr=pt->children->head->addr;
            break;

        case  16  :
            pt->addr=NULL;
            break;

        case  17  :

            break;

        case  18  :

            break;

        case  19  :
            pt->addr=NULL;
            break;

        case  20  :

            break;

        case  21  :

            break;

        case  22  :

            break;

        case  23  :

            break;

        case  24  :
            pt->addr=NULL;
            break;

        case  25  :

            break;

        case  26  :
            pt->addr=NULL;
            break;

        case  27  :

            break;

        case  28  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  29  :
            pt->addr=NULL;
            break;

        case  30  :

            break;

        case  31  :
            pt->addr=NULL;
            break;

        case  32  :
            pt->addr=pt->children->head->addr;
            break;

        case  33  :
            pt->addr=pt->children->head->addr;
            break;

        case  34  :
            pt->addr=pt->children->head->addr;
            break;

        case  35  :
            pt->addr=pt->children->head->addr;
            break;

        case  36  :
            pt->addr=pt->children->head->addr;
            break;

        case  37  :

            break;

        case  38  :

            break;

        case  39  :
            pt->addr=NULL;
            break;

        case  40  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  41  :

            break;

        case  42  :
            pt->addr=pt->children->head->addr;
            break;

        case  43  :
            pt->addr=NULL;
            break;

        case  44  :
            pt->addr=pt->children->head->addr;
            break;

        case  45  :

            break;

        case  46  :

            break;

        case  47  :

            break;

        case  48  :
            pt->addr=NULL;
            break;

        case  49  :

            break;

        case  50  :

            break;

        case  51  :

            break;

        case  52  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  53  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  54  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  55  :
            pt->addr=NULL;
            break;

        case  56  :

            break;

        case  57  :

            break;

        case  58  :
            
            break;

        case  59  :

            break;

        case  60  :

            break;

        case  61  :

            break;

        case  62  :
            pt->addr=pt->children->head->addr;
            break;

        case  63  :
            pt->addr=pt->children->head->addr;
            break;

        case  64  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  65  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  66  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  67  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  68  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  69  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  70  :

            break;

        case  71  :
            pt->addr=NULL;
            break;

        case  72  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  73  :

            break;

        case  74  :

            break;

        case  75  :

            break;

        case  76  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  77  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  78  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  79  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  80  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  81  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  82  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  83  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  84  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  85  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  86  :
            pt->addr=makeASTLeaf(pt->children->head->lu);
            break;

        case  87  :

            break;

        case  88  :
            pt->addr=pt->children->head->addr;
            break;

        case  89  :
            pt->addr=NULL;
            break;

        case  90  :

            break;

        case  91  :
            pt->addr=pt->children->head->addr;
            break;

        case  92  :
            pt->addr=NULL;
            break;


    }
}