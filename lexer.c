#include <stdio.h>
#include "enumVar.h"

extern int line;  // memoryPool.c
extern char *src; // tokenizer.c
extern char *lastPos;
extern int *currentId;
extern int token;
extern int tokenVal;

extern int nextToken();
extern void initSymbolTab();

char *map[] = {
    "char", "else", "enum", "if", "int", "return", "sizeof", "while",
    "=", "?", "|", "&", "||", "^", "&&", "==", "!=", "<", ">", "<=",
    ">=", "<<", ">>", "+", "-", "*", "/", "%%", "++", "--", "["};

void lexicalAnalysis()
{
    int ret;
    int i;
    char *p;
    initSymbolTab();
    while (token > 0)
    {
        ret = nextToken();

        if (ret == OP)
        {
            i = token - Char;
            if (i >= 0)
                printf("Line %d: %s ---- <operator>\n\n", line, map[i]);
            else if (token == '!' || token == ']' || token == '~')
                printf("Line %d: %c ---- <operator>\n\n", line, token);
        }
        else if (ret == DELIM)
        {
            printf("Line %d: %c ---- <delimiter>\n\n", line, token);
        }
        else if (ret == CONST)
        {
            if (token == '"')
            {
                p = (char *)tokenVal;
                printf("Line %d: \"%s\" ---- <constant>\n\n", line, p);
            }
            else
                printf("Line %d: (int)%d ---- <constant>\n\n", line, tokenVal);
        }
        else if (ret == ID)
        {
            p = lastPos;
            i = currentId[Len];
            printf("Line %d: %.*s ---- <identifier>\n\n", line, i, p);
        }
        else if (ret == KEY)
        {
            i = token - Char;
            printf("Line %d: %s ---- <keyword>\n\n", line, map[i]);
        }
    }
}