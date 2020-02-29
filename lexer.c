#include <stdio.h>
#include <string.h>
#include "enumVar.h"

extern int line, poolSize;           // memoryPool.c
extern char *src, *oldSrc, *lastPos; // tokenizer.c
extern long long *currentId, *symbols, token, tokenVal;
extern int nextToken();
// help to print

char *map[] = {
    "void", "int", "float", "char", "else", "enum", "if", "return",
    "sizeof", "while", "for", "Break", "Continue",
    "=", "?", "|", "&", "||", "^", "&&", "==", "!=", "<", ">", "<=",
    ">=", "<<", ">>", "+", "-", "*", "/", "%%", "++", "--", "["};

// initialize symbol table
void initSymbolTab()
{
    int i;

    line = 1;
    memset(symbols, 0, poolSize << 2);

    src = "void int float char else enum if return sizeof while for break continue";
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
                printf("Line %d: %s ---- <operator>\n", line, map[i]);
            else if (token == '!' || token == ']' || token == '~')
                printf("Line %d: %c ---- <operator>\n", line, token);
        }
        else if (ret == DELIM)
            printf("Line %d: %c ---- <delimiter>\n", line, token);
        else if (ret == CONST_STR)
            printf("Line %d: \"%s\" ---- <constant>\n", line, (char *)tokenVal);
        else if (ret == CONST_INT)
            printf("Line %d: (int)%d ---- <constant>\n", line, tokenVal);
        else if (ret == CONST_FLO)
            printf("Line %d: (float)%s ---- <constant>\n", line, (char *)tokenVal);
        else if (ret == ID)
        {
            p = (char *)currentId[Name];
            i = currentId[Len];
            printf("Line %d: %.*s ---- <identifier>\n", line, i, p);
        }
        else if (ret == KEY)
        {
            i = token - Void;
            printf("Line %d: %s ---- <keyword>\n", line, map[i]);
        }
    }
}