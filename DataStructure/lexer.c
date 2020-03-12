#include <stdio.h>
#include <string.h>
#include "enumVar.h"

extern int poolSize, line;           // memoryPool.c
extern char *src, *oldSrc, *lastPos; // tokenizer.c
extern long long *currentId, *symbols, token, tokenVal;
extern int nextToken();
// help to print

char *map[] = {
    "void", "int", "float", "char", "else", "enum", "if", "return",
    "sizeof", "while", "for", "break", "continue",
    "=", "?", "||", "&&", "|", "^", "&", "==", "!=", "<", ">", "<=",
    ">=", "<<", ">>", "+", "-", "*", "/", "%", "++", "--", "["};

// initialize symbol table
void initSymbolTab()
{
    int i;

    line = 1;
    memset(symbols, 0, poolSize);

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
    int lastLine = 0;
    initSymbolTab();
    while (token > 0)
    {
        ret = nextToken();
        if (line != lastLine)
        {
            printf("Line %d\n", line);
            lastLine = line;
        }

        if (ret == OP)
        {
            i = token - Void;
            if (i >= 0)
                printf("    %s\t\toperator\n", map[i]);
            else if (token == '!' || token == ']' || token == '~')
                printf("    %c\t\toperator\n", token);
        }
        else if (ret == DELIM)
            printf("    %c\t\tdelimiter\n", token);
        else if (ret == CONST_STR)
            printf("    \"%s\"\t\tconstant\n", (char *)tokenVal);
        else if (ret == CONST_INT)
            printf("    (int)%d\t\tconstant\n", tokenVal);
        else if (ret == CONST_FLO)
            printf("    (float)%s\t\tconstant\n", (char *)tokenVal);
        else if (ret == ID)
        {
            p = (char *)currentId[Name];
            i = currentId[Len];
            printf("    %.*s\t\tidentifier\n", i, p);
        }
        else if (ret == KEY)
        {
            i = token - Void;
            printf("    %s\t\tkeyword\n", map[i]);
        }
    }
}