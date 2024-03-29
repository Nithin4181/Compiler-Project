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
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#include "lexer.h"

char* tokenTypeMap[] = {    
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RECORDID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_ERROR",
    "EPS",
    "DOLLAR"
};

#define SIZE_BUFFER 1024        // Size of input buffer used to read source code
#define LEXEME_SIZE 200         // Max size of lexeme
#define SLOT_COUNT 11           // No. of slots for hashing

char *previous_buffer;          // Previous buffer in twin buffer

char *current_buffer;           // Currently used buffer in the twin buffer

int line_no = 1;                // Current line number being read

int current_position = 0;       // Current position in the buffer being read

int end_file = 0;               // 1 => file end reached; 0 => file remaining to be read

Lookup* lookupTable;            // Lookup table for keywords

FILE *getStream(FILE *fp){
    /*Description: Returns the position of the file after reading a input chunk
    of size SIZE_BUFFER, till end of each line*/
    /*Arguments: File pointer*/
    /*Return Type: File pointer or NULL*/
    char *temp = previous_buffer;
    previous_buffer = current_buffer;
    current_buffer = temp;

    memset(current_buffer, 0 , sizeof(char)*(SIZE_BUFFER+1));

    if(feof(fp)){
		fclose(fp);
 		return NULL;
 	}
    
    int no_of_char;
    if ((no_of_char = fread(current_buffer,sizeof(char),SIZE_BUFFER,fp))>0){
        current_buffer[SIZE_BUFFER]='\0';
        return fp;
    }
    else {
        fclose(fp);
        return NULL;
    }
}

void addToken(Lexical_Unit* lu, Token_type type, char* lexeme, Val* value){
    /*Description: Initializes lexical unit with the given lexeme, token, and value*/
    /*Arguments: Lexical_Unit pointer, Token_type enum, Char pointer, Val union */
    /*Return Type: void*/
	lu->line_no = line_no;
	lu->token = type;
	lu->lexeme = lexeme;
	lu->val = value;
}

FILE *lexer_initialisation(char *sourceFile){
    /*Description: Initialize the Lexer*/
    /*Arguments: Source code file name*/
    /*Return Type: File pointer for reading source code*/  
    current_buffer=(char*)malloc(sizeof(char)*(SIZE_BUFFER+1));
    previous_buffer=(char*)malloc(sizeof(char)*(SIZE_BUFFER+1));

    memset(current_buffer, 0, sizeof(char)*(SIZE_BUFFER+1));
	memset(previous_buffer, 0, sizeof(char)*(SIZE_BUFFER+1));

    line_no=1;
    current_position=0;
    end_file = 0;

    lookupTable = createTable(SLOT_COUNT);

    FILE *fp=fopen(sourceFile,"r");

    if (fp==NULL){
        fprintf(stderr,"Error opening input code file\n");
        free(current_buffer);
        free(previous_buffer);
        return NULL;
    }
    return fp;
}

Lexical_Unit* getNextTokenUtil(FILE **fp, bool printErrors){
    /*Description: Get the next lexical token*/
    /*Arguments: Source code file pointer and a boolean used to decide whether or not to print lexical error messages*/
    /*Return Type: Lexical_Unit pointer*/
    if(end_file==1){
        if(*fp != NULL)   fclose(*fp);
        free(current_buffer);
        free(previous_buffer);
        return NULL;
    }
    
    Lexical_Unit *lu=(Lexical_Unit*)malloc(sizeof(Lexical_Unit));
    if (current_buffer[current_position]=='\0'){
        current_position = 0;
        *fp = getStream(*fp);
        if(*fp == NULL){
            end_file = 1;
            free(current_buffer);
            free(previous_buffer);
            return NULL;
        }
    }
    int state = 0;
    int final_state = 0;

    Val* val;

    char *lexeme = (char*)malloc(LEXEME_SIZE*sizeof(char));
    memset(lexeme, 0, LEXEME_SIZE*sizeof(char));

    int lexeme_position = 0;

    Node* node = NULL;

    while (1){
        switch (state){
            case 0:
                if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp = getStream(*fp);
                    if (*fp == NULL){
                        end_file = 1;
                        free(current_buffer);
                        free(previous_buffer);
                        return NULL;
                    }
                }
                else if (current_buffer[current_position]==' ' || current_buffer[current_position]=='\t'){  
                    state = 0;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\n'){   
                    line_no += 1;
                    current_position += 1;
                }
                else if (current_buffer[current_position]=='<'){
                    state = 1;
                    current_position++;
                }
                else if (current_buffer[current_position]=='%'){    
                    state = 2;
                    current_position++;
                }
                else if (current_buffer[current_position]=='a'||(current_buffer[current_position]>='e' && current_buffer[current_position]<='z')){
                    state = 3;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]>='b' && current_buffer[current_position]<='d'){
                    state = 4;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                    
                }
                else if (current_buffer[current_position]>='0' && current_buffer[current_position]<='9'){
                    state = 5;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='_'){
                    state = 6;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='#'){
                    state = 7;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='['){
                    state = 8;
                    current_position++;
                }
                else if (current_buffer[current_position]==']'){
                    state = 9;
                    current_position++;
                }
                else if (current_buffer[current_position]==','){
                    state = 10;
                    current_position++;
                }
                else if (current_buffer[current_position]==';'){
                    state = 11;
                    current_position++;
                }
                else if (current_buffer[current_position]==':'){
                    state = 12;
                    current_position++;
                }
                else if (current_buffer[current_position]=='.'){
                    state = 13;
                    current_position++;
                }
                else if (current_buffer[current_position]=='('){
                    state = 14;
                    current_position++;
                }
                else if (current_buffer[current_position]==')'){
                    state = 15;
                    current_position++;
                }
                else if (current_buffer[current_position]=='+'){
                    state = 16;
                    current_position++;
                }
                else if (current_buffer[current_position]=='-'){
                    state = 17;
                    current_position++;
                }
                else if (current_buffer[current_position]=='*'){
                    state = 18;
                    current_position++;
                }
                else if (current_buffer[current_position]=='/'){
                    state = 19;
                    current_position++;
                }
                else if (current_buffer[current_position]=='&'){
                    state = 20;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='@'){
                    state = 21;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='~'){
                    state = 22;
                    current_position++;
                }
                else if (current_buffer[current_position]=='='){
                    state = 23;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='>'){
                    state = 24;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='!'){
                    state = 25;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else{
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }          
            break;
           
            case 1:
                if (current_buffer[current_position]=='='){  
                    state = 26;
                    current_position++;
                }
                else if (current_buffer[current_position]=='-'){    
                    state = 27;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 33;                      
                        current_position++;
                        free(current_buffer);
                        free(previous_buffer);
                        return NULL;
                    }
                }
                else{
                   state = 33;                        
                   current_position++;
                }
                break;

            case 2:
                strcpy(lexeme,"%");
                addToken(lu, TK_COMMENT,lexeme, NULL);
                final_state = 1;
                while(current_buffer[current_position]!='\n'){   
                    if (current_buffer[current_position]=='\0'){
                        current_position=-1;
                        *fp=getStream(*fp);
                        if (*fp==NULL){
                            end_file=1;
                            current_position++;
                            break;
                        }
                    }
                    ++current_position;
                }
                if(current_buffer[current_position] == '\n')    
                break;

            case 3:
               if ((current_buffer[current_position]>='a' && current_buffer[current_position]<='z')){
                    state = 3;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 51;
                        current_position++;
                    }
                }
                else{
                    state = 51;
                    current_position++;
                }
                break;

            case 4:
                if ((current_buffer[current_position]>='a' && current_buffer[current_position]<='z')){
                    state = 3;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]>='2' && current_buffer[current_position]<='7'){
                    state = 30;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 51;
                        current_position++;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Wrong Identifier name %s\n", line_no, lexeme);
                    state = 52;
                }
                break;
           
            case 5:
                if ((current_buffer[current_position]>='0' && current_buffer[current_position]<='9')){
                    state = 5;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='.'){
                    state = 35;         
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 34;
                        current_position++;
                    }
                }
                else{
                    state = 34;
                    current_position++;
                }
                break;
            
            case 6:
                if ((current_buffer[current_position]>='a' && current_buffer[current_position]<='z') || (current_buffer[current_position]>='A' && current_buffer[current_position]<='Z')){
                    state = 38;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Wrong Identifier name %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Wrong Identifier name %s\n", line_no, lexeme);
                    state = 52;
                }
                break;
            
            case 7:
                if (current_buffer[current_position]>='a' && current_buffer[current_position]<='z'){
                    state = 41;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Wrong Identifier name %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Wrong Identifier name %s\n", line_no, lexeme);
                    state = 52;
                }
                break;
            
            case 8:
                strcpy(lexeme,"[");
                addToken(lu, TK_SQL,lexeme, NULL);
                final_state=1;
                break;

            case 9:
                strcpy(lexeme,"]");
                addToken(lu, TK_SQR,lexeme, NULL);
                final_state=1;
                break;

            case 10:
                strcpy(lexeme,",");
                addToken(lu, TK_COMMA,lexeme, NULL);
                final_state=1;
                break;

            case 11:
                strcpy(lexeme,";");
                addToken(lu, TK_SEM,lexeme, NULL);
                final_state=1;
                break;

            case 12:
                strcpy(lexeme,":");
                addToken(lu, TK_COLON,lexeme, NULL);
                final_state=1;
                break;

            case 13:
                strcpy(lexeme,".");
                addToken(lu, TK_DOT,lexeme, NULL);
                final_state=1;
                break;

            case 14:
                strcpy(lexeme,"(");
                addToken(lu, TK_OP,lexeme, NULL);
                final_state=1;
                break;

            case 15:
                strcpy(lexeme,")");
                addToken(lu, TK_CL,lexeme, NULL);
                final_state=1;
                break;

            case 16:
                strcpy(lexeme,"+");
                addToken(lu, TK_PLUS,lexeme, NULL);
                final_state=1;
                break;

            case 17:
                strcpy(lexeme,"-");
                addToken(lu, TK_MINUS,lexeme, NULL);
                final_state=1;
                break;

            case 18:
                strcpy(lexeme,"*");
                addToken(lu, TK_MUL,lexeme, NULL);
                final_state=1;
                break;

            case 19:
                strcpy(lexeme,"/");
                addToken(lu, TK_DIV,lexeme, NULL);
                final_state=1;
                break;

            case 20:
                if (current_buffer[current_position]=='&'){
                    state = 43;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 21:
                if (current_buffer[current_position]=='@'){
                    state = 45;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 22:
                strcpy(lexeme,"~");
                addToken(lu, TK_NOT,lexeme, NULL);
                final_state=1;
                break;

            case 23:
                if (current_buffer[current_position]=='='){
                    state = 47;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 24:
                if (current_buffer[current_position]=='='){
                    state = 49;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 48;
                        current_position++;
                    }
                }
                else{
                    state = 48;
                    current_position++;
                }
                break;

            case 25:
                if (current_buffer[current_position]=='='){
                    state = 50;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        end_file=1;
                        state = 52;
                    }
               }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 33:
                strcpy(lexeme,"<");
                addToken(lu, TK_LT,lexeme, NULL);
                final_state=1;
                --current_position;             // Retraction state
                break;

            case 26:
                strcpy(lexeme,"<=");
                addToken(lu, TK_LE,lexeme, NULL);
                final_state=1;
                break;
           
            case 27:
                if (current_buffer[current_position]=='-'){
                    state = 28;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        strcpy(lexeme,"<-");
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        end_file=1;
                        state = 52;
                    }
               }
                else{
                    strcpy(lexeme,"<-");
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 28:
                if(current_buffer[current_position]=='-'){
                    state = 29;
                    current_position++;
                }
                else
                {
                    strcpy(lexeme,"<--");
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 29:
                strcpy(lexeme,"<---");
                addToken(lu, TK_ASSIGNOP,lexeme, NULL);
                final_state=1;
                break;

           case 30:
                if(current_buffer[current_position]>='b' && current_buffer[current_position]<='d'){
                    state = 30;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if(current_buffer[current_position]>='2' && current_buffer[current_position]<='7'){
                    state = 31;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
               else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file=1;
                        state = 32;
                        current_position++;        
                    }
                }
                else{
                    state = 32;
                    current_position++;
                }
                break;

            case 31:
                if(current_buffer[current_position]>='2' && current_buffer[current_position]<='7'){
                    state = 31;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file=1;
                        state = 32;
                        current_position++;
                    }
                }
                else{
                    state = 32;
                    current_position++;
                }
                break;

            case 32:
                if(strlen(lexeme) > 20){
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Identifier name %s is too long\n", line_no, lexeme);
                    state = 52;
                }
                else{                    
                    addToken(lu, TK_ID,lexeme, NULL);
                    final_state = 1;
                }
                --current_position;             //  Retraction state
                break;

            case 34:
                val = (Val*)malloc(sizeof(Val));
                val->integer = atoi(lexeme);
                addToken(lu, TK_NUM,lexeme, val);
                final_state = 1;
                --current_position;             //  Retraction state
                break;
            
            case 35:
                if(current_buffer[current_position]>='0' && current_buffer[current_position]<='9'){
                    state = 36;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Invalid real number %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Invalid real number %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 36:
                if(current_buffer[current_position]>='0' && current_buffer[current_position]<='9'){
                    state = 37;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Invalid real number %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Invalid real number %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 37:
                val = (Val*)malloc(sizeof(Val));
                val->real = atof(lexeme);
                addToken(lu, TK_RNUM,lexeme, val);
                final_state = 1;
                break;
            
            case 38:
                if((current_buffer[current_position]>='a' && current_buffer[current_position]<='z') || (current_buffer[current_position]>='A' && current_buffer[current_position]<='Z')){
                    state = 38;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if(current_buffer[current_position]>='0' && current_buffer[current_position]<='9'){
                    state = 39;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 40;
                        current_position++;
                    }
                }
                else{
                    state = 40;
                    current_position++;
                }
                break;

            case 39:
                if(current_buffer[current_position]>='0' && current_buffer[current_position]<='9'){
                    state = 39;
                    lexeme[lexeme_position] = current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 40;
                        current_position++;
                    }
                }
                else{
                    state = 40;
                    current_position++;
                }
                break;
            
            case 40:
                node = getTokenType(lexeme,lookupTable);
                if(node != NULL){
                    addToken(lu, node->token, node->lexeme, NULL);
                    final_state = 1;
                }
                else if(strlen(lexeme) > 30){
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Function name %s is too long\n", line_no, lexeme);
                    state = 52;
                }        
                else{
                    addToken(lu, TK_FUNID,lexeme, NULL);
                    final_state = 1;
                }   
                --current_position;     // Retraction state
                break;
            
            case 41:
                if (current_buffer[current_position]>='a' && current_buffer[current_position]<='z'){
                    state = 41;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        state = 42;
                        current_position++;
                    }
                }
                else{
                    state = 42;
                    current_position++;
                }
                break;
            
            case 42:
                addToken(lu, TK_RECORDID,lexeme, NULL);
                final_state = 1;
                --current_position;     // Retraction state
                break;
            
            case 43:
                if (current_buffer[current_position]=='&'){
                    state = 44;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;

            case 44:
                addToken(lu, TK_AND,lexeme, NULL);
                final_state = 1;
                break;
            
            case 45:
                if (current_buffer[current_position]=='@'){
                    state = 46;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=='\0'){
                    current_position = 0;
                    *fp=getStream(*fp);
                    if (*fp==NULL){
                        end_file = 1;
                        if(printErrors)
                            fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                        state = 52;
                    }
                }
                else{
                    if(printErrors)
                        fprintf(stderr, "Line %d\t| Lexical Error: Unknown pattern %s\n", line_no, lexeme);
                    state = 52;
                }
                break;
            
            case 46:
                addToken(lu, TK_OR,lexeme, NULL);
                final_state = 1;
                break;
            
            case 47:
                addToken(lu, TK_EQ,lexeme, NULL);
                final_state = 1;
                break;

            case 48:
                addToken(lu, TK_GT,lexeme, NULL);
                final_state = 1;
                --current_position;     // Retraction state
                break;

            case 49:
                addToken(lu, TK_GE,lexeme, NULL);
                final_state = 1;
                break;
            
            case 50:
                addToken(lu, TK_NE,lexeme, NULL);
                final_state = 1;
                break;
            
            case 51:
                node = getTokenType(lexeme,lookupTable);
                if(node != NULL){
                    addToken(lu, node->token, node->lexeme, NULL);
                    final_state = 1;
                }
                else{
                    addToken(lu, TK_FIELDID,lexeme, NULL);
                    final_state = 1;
                }
                --current_position;
                break;
            
            case 52:
                addToken(lu, TK_ERROR, lexeme, NULL);
                final_state = 1;
                break;
        }
        if(final_state == 1)
            break;
    } 
    return lu;
}

inline Lexical_Unit* getNextToken(FILE** fp){
    /*Description: Gets next token and print lexical errors*/
    /*Arguments: Source file pointer*/
    /*Return Type: Lexical_Unit pointer*/
    return getNextTokenUtil(fp, true);
}

inline Lexical_Unit* getNextToken_NoError(FILE** fp){
    /*Description: Gets next token without printing lexical errors*/
    /*Arguments: Source file pointer*/
    /*Return Type: Lexical_Unit pointer*/
    return getNextTokenUtil(fp, false);
}

void removeComments(char *testcaseFile){
    /* Description: Remove comments from the source code and output to console*/
    /* Arguments: Char pointer*/
    /* Return type: void */
	current_buffer = (char*)malloc(sizeof(char)*(SIZE_BUFFER+1));
	previous_buffer = (char*)malloc(sizeof(char)*(SIZE_BUFFER+1));
	
	memset(current_buffer, 0, sizeof(char)*(SIZE_BUFFER+1));
	memset(previous_buffer, 0, sizeof(char)*(SIZE_BUFFER+1));

	FILE* input = fopen(testcaseFile, "r");
    
	char ch;
    int line = 1;
    current_position = 0;
	
	if(input==NULL){
		fprintf(stderr,"Error Opening Source Code File\n");
		return;
	}

	input = getStream(input);
	int i = 0;
    int file_end = 0; 
	int emptyline = 1;

	while(1){
		if(current_buffer[i]=='\0'){
			input = getStream(input);
			if(input==NULL){
				break;
			}
			i=0;
		}		
		if(current_buffer[i]=='%'){
			i++;
			while(current_buffer[i]!='\n'){
				if(current_buffer[i]=='\0'){
					input = getStream(input);
					if(input==NULL){
						file_end = 1;
						break;
					}
					i = -1;
				}
				i++;
			}
		}
		if(file_end==1)
			break;
		if(current_buffer[i]!='\n')
			emptyline = 0;
		if(!(emptyline==1 && current_buffer[i]=='\n')){
			printf("%c",current_buffer[i]);		
		}
		if(current_buffer[i]=='\n')
			emptyline = 1;
		i++;
	}
    printf("\n");
	if(input!=NULL)
		fclose(input);
    free(current_buffer);
    free(previous_buffer);
}

void printTokenList(char *sourceFile){
    /* Description: Print the token list from the source file */
    /* Arguments: Source code file name */
    /* Return type: void */
    FILE *fp = lexer_initialisation(sourceFile);
	Lexical_Unit* lu;
	lu = getNextToken_NoError(&fp);
	int i = 1;
    printf("\tLine\t\t\t\tLexeme \t\t\t\tToken Type\n");
    printf("######################################################################################\n\n");
	while(lu!=NULL){
        printf("\t  %-3d%40s\t\t%20s\n",lu->line_no, lu->lexeme, tokenTypeMap[lu->token]);
		lu = getNextToken_NoError(&fp);
		i++;
	}
    printf("\n");
	if(fp!=NULL)
		fclose(fp);	
}