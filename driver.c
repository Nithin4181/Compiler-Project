/*
Group No: 26
Authors:
Naveen Unnikrishnan - 2016A7PS0111P
Adithya Mamallan - 2016A7PS0028P
Nithin Myppan Benny - 2016A7PS0014P
Swarup N - 2016A7PS0080P
*/
#include "lexer.h"
#include "parser.h"
#include <time.h>

int main(int argc, char* argv[]){
    printf("\nStage 1 - Implementation Status\n");
    printf("> Grammar loads dynamically from grammar.txt\n");
    printf("> First and follow set computation automated by code\n");
    printf("> Both lexical and syntax analysis modules implemented\n\n");
    printf("********************************\n\n");

    if(argc!=3){
        printf("Please re-enter command as ./stage1exe testcase.txt parsetreeOutFile.txt\n");
        return 0;
    }

    Grammar *g = loadGrammar("grammar.txt");
    printf("> Grammar loaded.\n");
    FirstAndFollow* ff = getFirstAndFollowSets(g);
    printf("> First and follow sets computed.\n");
    ParsingTable pt = makeNewParseTable();
    createParseTable(g, ff, pt);
    printf("> Parse table created.\n\n");
    printf("********************************\n\n");
    int loop = 1, errors;
    ParseTree tree;
    while(loop){
        printf("Enter the task to perform.\n\n");
        printf("0 - Quit\n");
        printf("1 - Remove comments from the input code and print\n");
        printf("2 - Print the token list generated by the lexer\n");
        printf("3 - Parse to verify the syntactic correctness of the input code and print the parse tree appropriately\n");
        printf("4 - Print the total time taken by Stage 1 of the compiler\n\n");
        
        int op;
        scanf("%d", &op);

        switch(op){
            case 0:
                loop = 0;
                break;
            case 1:
                printf("\n>> Removing comments.\n********************************\n\n");
                removeComments(argv[1]);
                break;
            case 2:
                printf("\n>> Printing token list.\n********************************\n\n");
                printTokenList(argv[1]);
                break;
            case 3:
                tree = parseInputSourceCode(argv[1],pt,ff,&errors);
                printParseTree(tree, argv[2]);
                break;
            case 4:
                printf("\n>>Beginning timing.\n");
                clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                start_time = clock();
                Grammar *g2 = loadGrammar("grammar.txt");
                printf("> Grammar loaded.\n");
                FirstAndFollow* ff2 = getFirstAndFollowSets(g2);
                printf("> First and follow sets computed.\n");
                ParsingTable pt2 = makeNewParseTable();
                createParseTable(g2, ff2, pt2);
                printf("> Parse table created.\n\n");
                tree = parseInputSourceCode(argv[1],pt,ff,&errors);
                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total time taken = %f cycles = %f seconds\n\n", total_CPU_time, total_CPU_time_in_seconds);
                break;
            default:
                printf("Invalid argument, please try again.\n");
        }
    }
    return 0;
}