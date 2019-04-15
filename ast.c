/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
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
    "ID_LIST",
    "RECORD"
};

AST makeAST(ParseTree pt){
    if (pt==NULL)   return NULL;

    makeAST_postOrderTraversal(pt);

    AST ast=(AST)malloc(sizeof(ASTNode));
    ast=pt->addr;

}

void makeAST_postOrderTraversal(ParseTree pt)
{
    if (pt==NULL)   return;

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

void addASTChildren(ASTChildren *ch, AST ast){
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
    
ASTNode* makeASTNode(astNodeName label, bool leaf, ASTNode* parent, ASTChildren* children, ASTNode* next, Lexical_Unit* lu, bool global){
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->label = label;
    node->leaf = leaf;
    node->parent = parent;
    node->children = children;
    node->next = next;
    node->lu = lu;
    node->global = global;

    node->currentScope = NULL;
    node->token = -1;
    node->returnTypes = NULL;

    if(node->leaf)
        node->lineNo = node->lu->line_no;

    node->value = NULL;

    return node;
}

inline ASTNode* makeASTLeaf(Lexical_Unit* lu, bool global){
    return makeASTNode(-1,true,(ASTNode*)NULL,(ASTChildren*)NULL,(ASTNode*)NULL,lu, global);
}

void addParentPointers(ASTNode* parent, ASTChildren* children){
    if(children->head == NULL)
        return;
    ASTNode* temp1 = children->head;
    ASTNode* temp2;

    while(temp1 != NULL){
        temp1->parent = parent;
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
    ASTChildren* children;
    switch(ruleNo){
        case  1  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
			addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(PROGRAM,false,NULL,children,NULL,NULL,false);
			addParentPointers(pt->addr,children);
            break;

        case  2  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(MAIN,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  3  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
			addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(OTHER_FN,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  4  :
            pt->addr=NULL;
            break;

        case  5  :
            children = initializeASTChildren(); 
			addASTChildren(children, makeASTLeaf(pt->children->head->lu, false));
            addASTChildren(children, pt->children->head->next->addr);
            addASTChildren(children, pt->children->head->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->addr);
            pt->addr = makeASTNode(FN,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  6  :
            pt->addr=pt->children->head->next->next->next->next->addr;
            break;

        case  7  :
            pt->addr=pt->children->head->next->next->next->next->addr;
            break;

        case  8  :
            pt->addr=NULL;
            break;

        case  9  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, makeASTLeaf(pt->children->head->next->lu, false));
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(PAR_LIST,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  10  :
            pt->addr=pt->children->head->addr;
            break;

        case  11  :
            // pt->addr=pt->children->head->addr;

            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
			pt->addr = makeASTNode(RECORD,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  12  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  13  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  14  :
            pt->addr=makeASTLeaf(pt->children->head->next->lu, false);
            break;

        case  15  :
            pt->addr=pt->children->head->next->addr;
            break;

        case  16  :
            pt->addr=NULL;
            break;

        case  17  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->addr);
            addASTChildren(children, pt->children->head->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->addr);
			pt->addr = makeASTNode(STMTS,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  18  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(TYPE_DEFS,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  19  :
            pt->addr=NULL;
            break;

        case  20  :
            children = initializeASTChildren(); 
            addASTChildren(children, makeASTLeaf(pt->children->head->next->lu, false));
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(TYPE_DEF,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  21  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->addr);
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(FIELD_DEFS,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  22  :
            children = initializeASTChildren(); 
            addASTChildren(children, pt->children->head->next->addr);
            addASTChildren(children, makeASTLeaf(pt->children->head->next->next->next->lu, false));
			pt->addr = makeASTNode(FIELD_DEF,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  23  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(MORE_FIELDS,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  24  :
            pt->addr=NULL;
            break;

        case  25  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(DECLARATIONS,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  26  :
            pt->addr=NULL;
            break;

        case  27  :                     
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->next->addr);
            if(pt->children->head->next->next->next->next->addr == NULL){              
			    addASTChildren(children, makeASTLeaf(pt->children->head->next->next->next->lu, false));
                pt->addr = makeASTNode(DECLARATION,false,NULL,children,NULL,NULL,false); 
            }
            else{           
                addASTChildren(children, makeASTLeaf(pt->children->head->next->next->next->lu, true));
                pt->addr = makeASTNode(DECLARATION,false,NULL,children,NULL,NULL,true); 
                free(pt->children->head->next->next->next->next->addr);
                pt->children->head->next->next->next->next->addr = NULL;
            }
			addParentPointers(pt->addr,children);
            break;

        case  28  :
            pt->addr = malloc(sizeof(ASTNode));
            break;

        case  29  :
            pt->addr=NULL;
            break;

        case  30  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(OTHER_STMTS,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
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
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(ASSIGNMENT_STMT,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  38  :
            children = initializeASTChildren(); 
			addASTChildren(children, makeASTLeaf(pt->children->head->lu, false));
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(SINGLE_OR_REC_ID,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  39  :
            pt->addr=NULL;
            break;

        case  40  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  41  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->addr);
            addASTChildren(children,makeASTLeaf(pt->children->head->next->next->lu, false));
            addASTChildren(children, pt->children->head->next->next->next->next->next->addr);
			pt->addr = makeASTNode(FUN_CALL_STMT,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  42  :
            pt->addr=pt->children->head->next->addr;
            break;

        case  43  :
            pt->addr=NULL;
            break;

        case  44  :
            pt->addr=pt->children->head->next->addr;
            break;

        case  45  :
            children = initializeASTChildren(); 
            addASTChildren(children, pt->children->head->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->next->addr);
			pt->addr = makeASTNode(ITERATIVE_STMT,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  46  :
            children = initializeASTChildren(); 
            addASTChildren(children, pt->children->head->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->next->next->next->addr);
			pt->addr = makeASTNode(CONDITIONAL_STMT,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  47  :
            children = initializeASTChildren(); 
			addASTChildren(children, pt->children->head->next->addr);
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(ELSE,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  48  :
            pt->addr=NULL;
            break;

        case  49  :
            children = initializeASTChildren(); 
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(IO_STMT_READ,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  50  :
            children = initializeASTChildren(); 
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(IO_STMT_WRITE,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  51  :
            children = initializeASTChildren();
            addASTChildren(children, makeASTLeaf(pt->children->head->lu, false)); 
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(ALL_VAR,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children);
            break;

        case  52  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  53  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  54  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  55  :
            pt->addr=NULL;
            break;

        case  56  :
            arithmeticASTTraversal(pt, NULL);
            break;

        case  57  :
            pt->addr = NULL;
            break;

        case  58  :
            pt->addr = NULL;
            break;

        case  59  :
            arithmeticASTTraversal(pt, NULL);
            break;

        case  60  :
            pt->addr = NULL;
            break;

        case  61  :
            pt->addr = NULL;
            break;

        case  62  :
            pt->addr=pt->children->head->next->addr;
            break;

        case  63  :
            pt->addr=pt->children->head->addr;
            break;

        case  64  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  65  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  66  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  67  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  68  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  69  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  70  :
            children = initializeASTChildren();
            addASTChildren(children, makeASTLeaf(pt->children->head->lu, false)); 
            addASTChildren(children, pt->children->head->next->addr);
			pt->addr = makeASTNode(ALL,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children); 
            break;

        case  71  :
            pt->addr=NULL;
            break;

        case  72  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  73  :
            children = initializeASTChildren();
            addASTChildren(children, pt->children->head->next->addr); 
            addASTChildren(children, pt->children->head->next->next->next->addr);
            addASTChildren(children, pt->children->head->next->next->next->next->next->addr);
			pt->addr = makeASTNode(BOOLEAN1,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children); 
            break;

        case  74  :
            children = initializeASTChildren();
            addASTChildren(children, pt->children->head->addr); 
            addASTChildren(children, pt->children->head->next->addr);
            addASTChildren(children, pt->children->head->next->next->addr);
			pt->addr = makeASTNode(BOOLEAN2,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children); 
            break;

        case  75  :
            children = initializeASTChildren();
            addASTChildren(children, pt->children->head->next->next->addr); 
			pt->addr = makeASTNode(BOOLEAN3,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children); 
            break;

        case  76  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  77  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  78  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  79  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  80  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  81  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  82  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  83  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  84  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  85  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  86  :
            pt->addr=makeASTLeaf(pt->children->head->lu, false);
            break;

        case  87  :
            children = initializeASTChildren();
            addASTChildren(children, pt->children->head->next->addr); 
            pt->addr = makeASTNode(RETURN,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children); 
            break;

        case  88  :
            pt->addr=pt->children->head->next->addr;
            break;

        case  89  :
            pt->addr=NULL;
            break;

        case  90  :
            children = initializeASTChildren();
            addASTChildren(children, makeASTLeaf(pt->children->head->lu, false)); 
            addASTChildren(children, pt->children->head->next->addr);
            pt->addr = makeASTNode(ID_LIST,false,NULL,children,NULL,NULL,false); 
			addParentPointers(pt->addr,children); 
            break;

        case  91  :
            pt->addr=pt->children->head->next->addr;
            break;

        case  92  :
            pt->addr=NULL;
            break;
    }
}

void arithmeticASTTraversal(ParseTree pt, ParseTree predecessor){
    if(pt == NULL)
        return;
    ParseTree curr, prev;
    Children* ch;
    int ruleNo = pt->ruleNo;
    switch(ruleNo){
        case 56:
            prev = NULL;
            ch = pt->children;
            curr = ch->head;            
            while(curr != NULL){
                arithmeticASTTraversal(curr, prev);
                prev = curr;
                curr = curr->next;
            }
            pt->addr = pt->children->head->next->addr;
            break;
        case 57:
            if(pt->parent->content->type.nonterm == arithmeticExpression)
                pt->inh_addr = predecessor->addr;
            else{
                ASTChildren* children = initializeASTChildren();
                addASTChildren(children, pt->parent->children->head->addr);
                addASTChildren(children, pt->parent->inh_addr);
                addASTChildren(children, predecessor->addr);
                pt->inh_addr = makeASTNode(ARITHMETIC, false, NULL, children, NULL, NULL, false);
                addParentPointers(pt->inh_addr, children);
            }
            prev = NULL;
            ch = pt->children;
            curr = ch->head;
            while(curr != NULL){
                arithmeticASTTraversal(curr, prev);
                prev = curr;
                curr = curr->next;
            }
            pt->addr = pt->children->head->next->next->addr;
            break;
        case 58:
            if(pt->parent->content->type.nonterm == arithmeticExpression)
                pt->inh_addr = predecessor->addr;
            else{
                ASTChildren* children = initializeASTChildren();
                addASTChildren(children, pt->parent->children->head->addr);
                addASTChildren(children, pt->parent->inh_addr);
                addASTChildren(children, predecessor->addr);
                pt->inh_addr = makeASTNode(ARITHMETIC, false, NULL, children, NULL, NULL, false);
                addParentPointers(pt->inh_addr, children);
            }
            pt->addr = pt->inh_addr;
            break;
        case 59:
            prev = NULL;
            ch = pt->children;
            curr = ch->head;            
            while(curr != NULL){
                arithmeticASTTraversal(curr, prev);
                prev = curr;
                curr = curr->next;
            }
            pt->addr = pt->children->head->next->addr;
            break;
        case 60:
            if(pt->parent->content->type.nonterm == term)
                pt->inh_addr = predecessor->addr;
            else{
                ASTChildren* children = initializeASTChildren();
                addASTChildren(children, pt->parent->children->head->addr);
                addASTChildren(children, pt->parent->inh_addr);
                addASTChildren(children, predecessor->addr);
                pt->inh_addr = makeASTNode(ARITHMETIC, false, NULL, children, NULL, NULL, false);
                addParentPointers(pt->inh_addr, children);
            }
            prev = NULL;
            ch = pt->children;
            curr = ch->head;
            while(curr != NULL){
                arithmeticASTTraversal(curr, prev);
                prev = curr;
                curr = curr->next;
            }
            pt->addr = pt->children->head->next->next->addr;
            break;
        case 61:
            if(pt->parent->content->type.nonterm == term)
                pt->inh_addr = predecessor->addr;
            else{
                ASTChildren* children = initializeASTChildren();
                addASTChildren(children, pt->parent->children->head->addr);
                addASTChildren(children, pt->parent->inh_addr);
                addASTChildren(children, predecessor->addr);
                pt->inh_addr = makeASTNode(ARITHMETIC, false, NULL, children, NULL, NULL, false);
                addParentPointers(pt->inh_addr, children);
            }
            pt->addr = pt->inh_addr;
            break;
    }
}

void displayAST(AST ast){

    printf("%20s %30s  %15s\n\n\n", "NODE TYPE", "PARENT TYPE", "IS_LEAF" );

    displayPreOrderAST(ast);
    printf("\n\n\n");
}

void displayPreOrderAST(AST ast){

    char *p;
    if (ast==NULL)  return ;

    if (ast->parent==NULL){
        p="None";
    }
    else{
        p=astNodeNameMap[ast->parent->label];        
    }

    if (ast->leaf){
        printf("%20s %30s  %15s\n", terminalMap[ast->lu->token],p,"yes");
    }
    else{
        printf("%20s %30s  %15s\n", astNodeNameMap[ast->label],p,"no");
    }

    ASTChildren *ch=ast->children;
    ASTNode* ch1=NULL;

    if (ch!=NULL){
        ch1=ch->head;

        while (ch1!=NULL){
            displayPreOrderAST(ch1);
            ch1=ch1->next;
        }
    }
}
    
void getParseTreeSize(ParseTree pt, int* count, int* size){
    if(pt == NULL)
        return;
    *size += sizeof(TreeNode);
    ++(*count);
    Children* ch = pt->children;
    if(ch != NULL){
        ParseTree pt2 = ch->head;
        while(pt2!=NULL){
            getParseTreeSize(pt2, count, size);
            pt2 = pt2->next;
        }
    }
}

void getASTSize(AST ast, int* count, int* size){
    if(ast == NULL)
        return;
    *size += sizeof(ASTNode);
    ++(*count);
    ASTChildren* ch = ast->children;
    if( ch != NULL){
        AST ast2 = ch->head;
        while(ast2 != NULL){
            getASTSize(ast2, count, size);
            ast2 = ast2->next;
        }
    }
}

void compressionRatio(ParseTree pt, AST ast){
    int ptNodes = 0, ptSize = 0, astNodes = 0, astSize = 0;
    getParseTreeSize(pt,&ptNodes,&ptSize);
    getASTSize(ast,&astNodes,&astSize);
	printf("\n\nPARSE TREE\n\tNumber of nodes = %d\n\tAllocated memory = %d bytes",ptNodes,ptSize);
	printf("\n\nABSTRACT SYNTAX TREE\n\tNumber of nodes = %d\n\tAllocated Memory = %d bytes\n\n", astNodes, astSize);

	float compressesionRatio = ((ptSize - astSize)/(float)ptSize)*(float)100;

	printf("Compression ratio: %f\n", compressesionRatio);
}