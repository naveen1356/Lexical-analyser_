#include"main.h"
int main(int argc,char *argv[])
{
    Lexical lexical;
    lexical.key_count=32;
    keywords(&lexical);
    if(argc!=2)
    {
        printf("Invalid no of arguments");
        return failure;
    }
    if(read_and_validate(argv,&lexical)==success)
    {
        printf("read and validate successful\n");
    }
    else{
        printf("read and validate unsuccessful\n");
        return failure;

    }
    
    if(operation_code(&lexical)==success)
    {
        printf("Lexical analysis successful\n");

    }
    else{
        printf("Lexical analysis unsuccessful\n");
        return failure;
    }


    return success;
    

}
Status read_and_validate(char *argv[],Lexical *lexical)
{
    if(strstr(argv[1],".c")!=0)
    {
        lexical->fname=argv[1];
        return success;
    }
    return failure;
}
Status open_file(Lexical *lexical)
{
    lexical->fptr=fopen(lexical->fname,"r");
    if(lexical->fptr==NULL)
    {
        printf("File Not found\n");
        return failure;
    }
    return success;

}
void keywords(Lexical *lexical)
{
     char key_words[32][10] = {"auto", "break", "case", "char", "const", "continue", "default", "do",
                                "double", "else", "enum", "extern", "float", "for", "goto", "if",
                                "int", "long", "register", "return", "short", "signed", "sizeof", "static",
                                "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
    for (int i=0; i<lexical->key_count; i++)
        strcpy(lexical->keywords[i], key_words[i]);

}
Status check_keyword(Lexical *lexical)
{
    for(int i=0;i<lexical->key_count;i++)
    {
        if(strcmp(lexical->keywords[i],lexical->buffer)==0)
            return is_keyword;
    }
    return is_identifier;
}

Status operation_code(Lexical *lexical)
{
    char ch;
    int i=0;
    int circle=0,square=0,flower=0;
    if((open_file(lexical))!=success) return failure;

    while((ch=fgetc(lexical->fptr))!=EOF)
    {
        if(ch=='#')         //header
        {
            while(ch !='\n' && ch !=EOF)
            {
                ch=fgetc(lexical->fptr);

            }
            continue;
        }
        //for identifier and keywords
        if(isalpha(ch))
        {
            lexical->buffer[i++]=ch;
            while(isalpha(ch=fgetc(lexical->fptr)))
            {
                lexical->buffer[i++]=ch;
            }
            lexical->buffer[i]='\0';
            i=0;
            fseek(lexical->fptr,-1,SEEK_CUR);
            if(check_keyword(lexical)==is_keyword)
            {
                printf("keyword\t:\t%s\n",lexical->buffer);

            }
            else{
                printf("Identifier\t:\t%s\n",lexical->buffer);
            }

        }
        
        else if(isdigit(ch)) 
        {
            if(ch=='0')
            {
                lexical->buffer[i++]=ch;
                char next_ch=fgetc(lexical->fptr);
                if(next_ch=='x' || next_ch=='X')
                {
                    lexical->buffer[i++]=next_ch;
                    ch=fgetc(lexical->fptr);
                    if(isxdigit(ch))
                    {
                        lexical->buffer[i++]=ch;
                        while(isxdigit(ch=fgetc(lexical->fptr)))
                            lexical->buffer[i++]=ch;
                        lexical->buffer[i++]='\0';
                        if(ch!=' ' && ch!=';' && ch!=',')
                        {
                            printf("number\t:Invalid hexadecimal number\n");
                            return  failure;
                        }
                        else{
                            printf("heaxadecimal number\t:%s\n",lexical->buffer);
                        }
                    }
                }
                else if(next_ch=='b' || next_ch=='B')
                {
                    lexical->buffer[i++]=next_ch;
                    ch=fgetc(lexical->fptr);
                    if(ch=='0' || ch=='1')
                    {
                        lexical->buffer[i++]=ch;
                        ch=fgetc(lexical->fptr);
                        lexical->buffer[i++]=ch;
                        while(ch =='0' || ch=='1')
                        {
                            ch=fgetc(lexical->fptr);
                            lexical->buffer[i++]=ch;
                            
                        }
                        lexical->buffer[i++]='\0';
                        if(ch!=' ' && ch!=';' && ch!=',')
                        {
                            printf("number\t:\tInvalid Binary number\n");
                            return  failure;
                        }
                        else{
                            printf("binary number\t:\t%s\n",lexical->buffer);
                        }

                    }

                }
                else{
                    while(isdigit(ch=fgetc(lexical->fptr)))
                    {
                        if(ch>'8')
                        {
                             printf("number\t:\tInvalid octal number\n");
                            return  failure;

                        }
                        lexical->buffer[i++]=ch;

                    }
                     lexical->buffer[i++]='\0';
                      printf("octal number\t:\t%s\n",lexical->buffer);



                }

            }
            else{
                int is_float=0;
                lexical->buffer[i++]=ch;
                while(isdigit(ch=fgetc(lexical->fptr)))
                    lexical->buffer[i++]=ch;
                if(ch=='.')
                {
                    is_float =1;
                     lexical->buffer[i++]=ch;
                    while(isdigit(ch=fgetc(lexical->fptr)))
                        lexical->buffer[i++]=ch;

                }
                lexical->buffer[i++]='\0';
                if(is_float==0)
                {
                    printf("number\t:\t%s\n",lexical->buffer);
                }
                else{
                    printf("float\t:\t%s\n",lexical->buffer);
                }
                
            }
            i=0;
            fseek(lexical->fptr,-1,SEEK_CUR);
        }
        //for operator
        else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>' || ch == '%' || 
                ch == '&' || ch == '|' || ch == '^' || ch == '!' || ch == '~')
        {
                    if(ch=='+')
                    {
                        lexical->buffer[i++]=ch;
                        char next_val=fgetc(lexical->fptr);
                        if(next_val=='+')
                        {
                            lexical->buffer[i++]=next_val;
                        }
                        else{
                             fseek(lexical->fptr,-1,SEEK_CUR);
                        }
                        lexical->buffer[i++]='\0';
                        printf("operator\t:\t%s\n",lexical->buffer);
                    }
                    else if(ch =='-'){
                          lexical->buffer[i++]=ch;
                        char next_val=fgetc(lexical->fptr);
                        if(next_val=='-')
                        {
                            lexical->buffer[i++]=next_val;
                        }
                        else{
                             fseek(lexical->fptr,-1,SEEK_CUR);
                        }
                        lexical->buffer[i++]='\0';
                        printf("operator\t:\t%s\n",lexical->buffer);
                    }
                    
                    else{
                        printf("operator\t:\t%c\n",ch);
                    }
                    i=0;
        }
        else if(ch==';' || ch==',' ||ch=='(' || ch==')' || ch=='[' || ch==']' || ch=='{' || ch=='}')
        {
                    if(ch=='(' || ch==')')
                    {
                        if(ch=='(') {
                            circle++;
                            printf("symbols\t:\t%c\n",ch);

                        }
                        else{
                            circle--;
                            printf("symbols\t:\t%c\n",ch);
                            if(circle<0) {
                                printf("Error closing )\n");
                                return failure;
                            }
                        }

                    }
                    else if(ch=='[' || ch==']')
                    {
                        if(ch=='[') {
                            square++;
                            printf("symbols\t:\t%c\n",ch);
                        }
                        else {
                            square--;
                            printf("symbols\t:\t%c\n",ch);
                            if(square<0) {
                                printf("Error closing ]\n");
                                return failure;
                            }

                        }

                    }
                    else if(ch=='{' || ch=='}')
                    {
                        if(ch=='{') {
                            flower++;
                            printf("symbols\t:\t%c\n",ch);
                        }
                        else{
                            flower--;
                            printf("symbols\t:\t%c\n",ch);
                            if(flower<0) {
                                printf("Error closing }\n");
                                return failure;
                            }

                        }

                    }
                else{
                     printf("symbols\t:\t%c\n",ch);

                }
           

        }
        else if(ch == '\'')
        {
            ch =fgetc(lexical->fptr);
            lexical->buffer[0]=ch;
            lexical->buffer[1]='\0';
            char close=fgetc(lexical->fptr);
            if(close=='\'')
            {
                printf("Character\t:\t%s\n",lexical->buffer);
            }
            else{
                printf("character not end with '\n");
                return failure;
            }

        }
        else if(ch == '\"')
        {
            int closed = 0;
            ch = fgetc(lexical->fptr);
            while(ch != '\"' && ch != '\n' && ch != EOF)
            {
                lexical->buffer[i++] = ch;
                ch = fgetc(lexical->fptr);
            }
            if(ch == '\"')
                closed = 1;
             lexical->buffer[i] = '\0';
            i= 0;
            if(closed)
            {
                printf("string literal\t:\t%s\n",lexical->buffer);

            }
            else{
                printf("string literal error\n");
                return failure;
            }

        }
        else if(isspace(ch))
        continue;
        

        
    }
    if(circle!=0 || square!=0 || flower!=0)
    {
        printf("Error,braces misallignment\n");
        return failure;
    }
    fclose(lexical->fptr);
        return success;
}