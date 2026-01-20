#ifndef MAIN_H
#define MAIN_H
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"types.h"


typedef struct {
    char buffer[40];
    FILE *fptr;
    char *fname;
    char keywords[32][10];
    int key_count;
    
}Lexical;
void keywords(Lexical *lexical);
Status check_keyword(Lexical *lexical);
Status open_file(Lexical *lexical);

Status read_and_validate(char *argv[],Lexical *lexical);
Status operation_code(Lexical *lexical);
#endif


