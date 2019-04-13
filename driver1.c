/*
Group No: 26
Authors:
    Nithin Benny Myppan - 2016A7PS0014P
    Adhitya Mamallan    - 2016A7PS0028P
    Swarup N            - 2016A7PS0080P
    Naveen Unnikrishnan - 2016A7PS0111P
*/
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include <time.h>

int main(int argc, char* argv[]){
    Grammar *g = loadGrammar("grammar.txt");
    FirstAndFollow* ff = getFirstAndFollowSets(g);
    ParsingTable pt = makeNewParseTable();
    createParseTable(g, ff, pt);
    int loop = 1, errors;
    ParseTree tree;
    tree = parseInputSourceCode(argv[1],pt,ff,&errors);
    AST ast = makeAST(tree);
    displayAST(ast);
    STTree st = makeSymbolTables(ast);
    displaySTTree(st);
    return 0;
}