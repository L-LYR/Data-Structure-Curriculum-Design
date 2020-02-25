#include <stdio.h>
#include <string.h>
#include "enumVar.h"

extern int line, poolSize;           // memoryPool.c
extern char *src, *oldSrc, *lastPos; // tokenizer.c
extern int *currentId, *symbols, token, tokenVal;
extern int nextToken();

char *map[] = {
    "void", "int", "char", "else", "enum", "if", "return", "sizeof", "while", "for", "Break", "Continue",
    "=", "?", "|", "&", "||", "^", "&&", "==", "!=", "<", ">", "<=",
    ">=", "<<", ">>", "+", "-", "*", "/", "%%", "++", "--", "["};

// initialize symbol table
void initSymbolTab()
{
    int i;

    line = 1;
    memset(symbols, 0, poolSize);

    src = "void int char else enum if return sizeof while for break continue";
    i = Void;
    while (i <= Continue)
    {
        nextToken();
        currentId[Token] = i++;
    }

    src = oldSrc;
}

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
            i = token - Void;
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
            p = (char *)currentId[Name];
            i = currentId[Len];
            printf("Line %d: %.*s ---- <identifier>\n\n", line, i, p);
        }
        else if (ret == KEY)
        {
            i = token - Void;
            printf("Line %d: %s ---- <keyword>\n\n", line, map[i]);
        }
    }
}