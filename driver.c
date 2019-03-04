#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]){
    Grammar *g = loadGrammar("grammar.txt");
    printGrammar(g);
    // if(argc > 1){
    //     removeComments(argv[1],"out.txt");
    // //     printTokenList(argv[1]);
    // }
    FirstAndFollow* ff = getFirstAndFollowSets(g);
    // printFirstAndFollow(ff);
    ParsingTable pt = makeNewParseTable();
    createParseTable(g, ff, pt);
    // printParsingTable(pt);
    int errors = 0;
    ParseTree tree = parseInputSourceCode(argv[1],pt,ff,&errors);
    printTokenList(argv[1]);
    printParseTree(tree, argv[2]);
    return 0;
}