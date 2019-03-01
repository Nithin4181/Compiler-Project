
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "lookUp.h"
#include "lexer.h"
#include "lexerDef.h"

char* tokenTypeMap[]=
{
"TK_ASSIGNOP",
"TK_COMMENT",
"TK_FIELDID"
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
"TK_NE"
}

#define SIZE_BUFFER 1024

char *previous_buffer;
char *current_buffer;

int line_no=1;

int current_position=0;

int end_file=0;

FILE *getStream(FILE *fp)
{
    char *temp=previous_buffer;
    previous_buffer=current_buffer;

    memset(current_buffer, 0 , sizeof(current_buffer));

    if(feof(fp)){
		fclose(fp);
 		return NULL;
 	}
    
    int no_of_char;
    if ((no_of_char=fread(current_buffer,sizeof(char),SIZE_BUFFER,fp))>0)
    {
        current_buffer[SIZE_BUFFER]='\0';
        return fp;
    }
    else 
    {
        fclose(fp);
        return NULL;
    }
	

}

void addToken(Lexical_Unit* lu, Token_type type, char* lexeme, Val value){
	lu->line_no = line_no;
	lu->token = type;
	lu-> lexeme = lexeme;
	lu-> val = value;
}


FILE *lexer_initialisation(char *input_code)
{
    current_buffer=(char*)malloc(sizeof(char)*(BUFFER_SIZE+1));
    previous_buffer=(char*)malloc(sizeof(char)*(BUFFER_SIZE+1));

    memset(current_buffer, 0, sizeof(current_buffer));
	memset(previous_buffer, 0, sizeof(previous_buffer));

    current line=1;
    current_position=0;


    //lookUpTable = newTable(NO_SLOTS);

    FILE *fp=fopen(input_code,"r");



    if (fp==NULL)
    {
        fprintf(stderr,"Error opening input code file\n");
        return NULL;
    }
    return fp;
}

Lexical_Unit* getNextToken(FILE **fp)
{
    if(end_file==1)
    {
        if(*fp!=NULL)   fclose(*fp);
        return NULL;
    }
    
    Lexical_Unit *lu=(Lexical_Unit*)malloc(sizeof(Lexical_Unit));

    if (current_buffer[current_position]=="\0"||current_buffer[current_position]==-1)
    {
        current_position=0;
        *fp=getStream(*fp);
        if(*fp==NULL)
        {
            end_file=1;
            return NULL;
        }
    }

    int state=0;
    int final_state=0;
    char *lexeme=(char*)malloc(40*sizeof(char));
    int lexeme_position=0;

   while (1)
   {
       switch (state)
       {
           case 0:
           {
                if (current_buffer[current_position]=="\0"||current_buffer[current_position]==-1)
                {
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL)
                    {
                        end_file=1;
                        return 
                    }

                }

                else if (current_buffer[current_position]==" "||current_buffer[current_position]=="\t")
                {
                    state=0;
                    current_position++;
                }
//doubt Naveen check
                else if (current_buffer[current_position]=="\n")
                {
                    line_no+=1;
                    current_position+=1;
                }
                else if (current_buffer[current_position]=="<")
                {
                    state=1;
                    current_position++;
                }
                else if (current_buffer[current_position]=="%")
                {
                    state=2;
                    current_position++;
                }
                else if (current_buffer[current_position]=="a"||(current_buffer[current_position]>'d' && current_buffer[current_position]<='z'))
                {
                    state=3;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]>="b"and current_buffer[current_position<="d"])
                {
                    state=4;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                    
                }
                else if (current_buffer[current_position]>="0"and current_buffer[current_position<="9"])
                {
                    
                    state=5;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=="_")
                {
                    state=6;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=="#")
                {
                    state=7;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]=="[")
                {
                    state=8;
                    current_position++;
                }
                else if (current_buffer[current_position]=="]")
                {
                    state=9;
                    current_position++;
                }
                else if (current_buffer[current_position]==",")
                {
                    state=10;
                    current_position++;
                }
                else if (current_buffer[current_position]==";")
                {
                    state=11;
                    current_position++;
                }
                else if (current_buffer[current_position]==":")
                {
                    state=12;
                    current_position++;
                }
                else if (current_buffer[current_position]==".")
                {
                    state=13;
                    current_position++;
                }
                else if (current_buffer[current_position]=="(")
                {
                    state=14;
                    current_position++;
                }
                else if (current_buffer[current_position]==")")
                {
                    state=15;
                    current_position++;
                }
                else if (current_buffer[current_position]=="+")
                {
                    state=16;
                    current_position++;
                }
                else if (current_buffer[current_position]=="-")
                {
                    state=17;
                    current_position++;
                }
                else if (current_buffer[current_position]=="*")
                {
                    state=18;
                    current_position++;
                }
                else if (current_buffer[current_position]=="/")
                {
                    state=19;
                    current_position++;
                }
                else if (current_buffer[current_position]=="&")
                {
                    state=20;
                    current_position++;
                }
                else if (current_buffer[current_position]=="@")
                {
                    state=21;
                    current_position++;
                }
                else if (current_buffer[current_position]=="~")
                {
                    state=22;
                    current_position++;
                }
                
                else if (current_buffer[current_position]=="=")
                {
                    state=23;
                    current_position++;
                }
                else if (current_buffer[current_position]==">")
                {
                    state=24;
                    current_position++;
                }
                else if (current_buffer[current_position]=="!")
                {
                    state=25;
                    current_position++;
                }                     
                               
           }break;
           
           case 1:
           {
               if (current_buffer[current_buffer]=="=")
               {
                   state=26;
                   current_position++;
               }
               else if (current_position=="-")
               {
                   state=27;
                   current_position++;
               }
               else if (current_buffer[current_position]=="\0"||current_buffer[current_position]==-1)
               {
                   current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL)
                    {
                        end_file=1;
                        state=28;
                        current_position++;
                        return 
                    }
               }
               else
               {
                   state=28;
                   current_position++;
               }
           }break;

           case 2:
           {
               strcpy(lexeme,"%");
               addToken(lu, TK_COMMENT,lexeme, NULL);
               final_state=1;
               
           }break;

           case 3:
           {
               if ((current_buffer[current_position]>='a' && current_buffer[current_position]<='z'))
                {
                    
                    state=3;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
//havent made the lookuptable
                else
                {

                }
           }break;
           
           case 4:
           {
               if ((current_buffer[current_position]>='a' && current_buffer[current_position]<='z'))
                {
                    state=3;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else if (current_buffer[current_position]>="2" && current_buffer[current_position]<="7")
                {
                    state=30;
                    lexeme[lexeme_position]=current_buffer[current_position];
                    lexeme_position++;
                    current_position++;
                }
                else
                {
                    fprintf(stderr, " Lexical Error, Wrong Identifier name %s at line %d\n", lexeme ,line_no);  
                }
           }break;
           
           case 5:
           {
               
           }break;
           
           case 6:
           {
               
           }break;
           
           case 7:
           {
               
           }break;
           
           case 8:
           {
               strcpy(lexeme,"[");
               addToken(lu, TK_SQL,lexeme, NULL);
               final_state=1;
           }break;
           
           case 9:
           {
               strcpy(lexeme,"]");
               addToken(lu, TK_SQR,lexeme, NULL);
               final_state=1;
           }break;

           case 10:
           {
               strcpy(lexeme,",");
               addToken(lu, TK_COMMA,lexeme, NULL);
               final_state=1;
           }break;

           case 11:
           {
               strcpy(lexeme,";");
               addToken(lu, TK_SEM,lexeme, NULL);
               final_state=1;
           }break;

           case 12:
           {
               strcpy(lexeme,":");
               addToken(lu, TK_COLON,lexeme, NULL);
               final_state=1;
               
           }break;
           case 13:
           {
               strcpy(lexeme,".");
               addToken(lu, TK_DOT,lexeme, NULL);
               final_state=1;
           }break;

           case 14:
           {
               strcpy(lexeme,"(");
               addToken(lu, TK_OP,lexeme, NULL);
               final_state=1;
           }break;

           case 15:
           {
               strcpy(lexeme,")");
               addToken(lu, TK_CL,lexeme, NULL);
               final_state=1;
           }break;

           case 16:
           {
               strcpy(lexeme,"+");
               addToken(lu, TK_PLUS,lexeme, NULL);
               final_state=1;
           }break;

           case 17:
           {
               strcpy(lexeme,"-");
               addToken(lu, TK_MINUS,lexeme, NULL);
               final_state=1;
           }break;

           case 18:
           {
               strcpy(lexeme,"*");
               addToken(lu, TK_MUL,lexeme, NULL);
               final_state=1;
           }break;

           case 19:
           {
               strcpy(lexeme,"/");
               addToken(lu, TK_DIV,lexeme, NULL);
               final_state=1;
           }break;

           case 20:
           {
               
           }break;

           case 21:
           {
               
           }break;

           case 22:
           {
               strcpy(lexeme,"~");
               addToken(lu, TK_NOT,lexeme, NULL);
               final_state=1;
           }break;

           case 23:
           {
               
           }break;

           case 24:
           {
               
           }break;

           case 25:
           {
               
           }break;

           case 26:
           {
               strcpy(lexeme,"<");
               addToken(lu, TK_LE,lexeme, NULL);
               final_state=1;
           }break;
           
           case 27:
           {
               if (current_buffer[current_position]=="-")
               {
                   state=28;
                   current_position++;
               }
               else if (current_buffer[current_position]=="\0"|)
               {
                    current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL)
                    {
                        strcpy(lexeme,"<-");
                        fprintf(stderr, " Lexical Error, Unknown pattern %s at line %d\n", lexeme ,line_no);
                        end_file=1;
                        return 
                    }
               }
               else
               {
                   strcpy(lexeme,"<-");
                   fprintf(stderr, " Lexical Error, Unknown pattern %s at line %d\n", lexeme ,line_no);
               }
               
           }break;

           case 28:
           {
               if(current_buffer[current_position]=="-")
               {
                   state=29;
                   current_position++;
               }
               else
               {
                   strcpy(lexeme,"<--");
                   fprintf(stderr, " Lexical Error, Unknown pattern %s at line %d\n", lexeme ,line_no);
               }
           }break;
           case 29:
           {
               strcpy(lexeme,"<---");
               addToken(lu, TK_ASSIGNOP,lexeme, NULL);
               final_state=1;
           }break;

           case 30:
           {
               if(current_buffer[current_position]>="b" && current_buffer[current_position]<="d")
               {
                   state=30;
                   lexeme[lexeme_position]=current_buffer[current_position];
                   lexeme_position++;
                   current_position++;
               }
               else if(current_buffer[current_position]>="2" && current_buffer[current_position]<="7")
               {
                   state=31;
                   lexeme[lexeme_position]=current_buffer[current_position];
                   lexeme_position++;
                   current_position++;
               }
               else if (current_buffer[current_position]=="\0"||current_buffer[current_position]==-1)
               {
                   current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL)
                    {
                        end_file=1;
                        state=32;
                        current_position++;
                        
                    }
               }
               else
               {
                   state=32;
                   current_position++;
               }
           }break;

           case 31:
           {
               if(current_buffer[current_position]>="2" && current_buffer[current_position]<="7")
               {
                   state=31;
                   lexeme[lexeme_position]=current_buffer[current_position];
                   lexeme_position++;
                   
                   current_position++;
               }
               else if (current_buffer[current_position]=="\0"||current_buffer[current_position]==-1)
               {
                   current_position=0;
                    *fp=getStream(*fp);
                    if (*fp==NULL)
                    {
                        end_file=1;
                        state=32;
                        current_position++;
                        
                    }
               }
               else
               {
                   state=32;
                   current_position++;
               }
           }break;


           


       }
   } 
    
}