#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]){
    Grammar *g = loadGrammar("grammar.txt");
    printGrammar(g);
    if(argc > 1){
        removeComments(argv[1],"out.txt");
        printTokenList(argv[1]);
    }
    FirstAndFollow* ff = getFirstAndFollowSets(g);
    printFirstAndFollow(ff);
    ParsingTable* pt = (ParsingTable*)malloc(sizeof(ParsingTable));
    createParseTable(g, ff, pt);
    print_parsing_table(pt);
    return 0;
}