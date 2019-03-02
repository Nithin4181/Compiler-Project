#include "lexer.h"

int main(int argc, char* argv[]){
    if(argc <= 1){
        printf("Not enough arguments!\n");
        return -1;
    }
    removeComments(argv[1],"out.txt");
    printTokenList(argv[1]);
    return 0;
}