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

void addASTChildren(ASTChildren *ch,AST * ast){
    if (ch->head==NULL)
    {
        ch->head=ast;
        ch->tail=ast;
    }
    else
    {
        ch->tail->next=ast;
        ch->tail=ch->tail->next;
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
    node->returnParameters = NULL;

    if(node->leaf)
        node->lineNo = node->lu->line_no;

    node->value = NULL;

    return node;
}

