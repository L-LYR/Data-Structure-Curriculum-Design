#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include <string.h>
extern char filename[64];

int main(int argc, char **argv)
{
    FILE *fp;
    int l, s, r, f; // flag for choosing operations
                    // lexical analysis, syntax analysis, rearrange source code
    int i;
    char **p;

    f = l = s = r = 0;
    i = argc - 1;
    p = argv + 1;

    if (i < 1 || i > 5)
    {
        printf("usage: mlpc <-operation> <filename> \n");
        printf("    -l  lexical analysis\n");
        printf("    -s  syntax analysis\n");
        printf("    -r  rearrange source code\n");
        printf("    -f  save all outputs in files\n");
        return -1;
    }

    while (i > 0 && **p == '-')
    {
        switch ((*p)[1])
        {
        case 'l':
            l = 1;
            break;
        case 's':
            s = 1;
            break;
        case 'r':
            r = 1;
            break;
        case 'f':
            f = 1;
            break;
        default:
            printf("Unknown operation!\n");
            return -1;
        }
        --i;
        ++p;
    }

    if (i < 1)
    {
        printf("Lack of filename!\n");
        return -1;
    }

    sprintf(filename, "%s", *p);
    readFromFile();
    allocMemoryPool();

    if (l)
    {
        if (f)
        {
            fp = freopen(strcat(filename, ".lex"), "w", stdout);
            if (fp == NULL)
            {
                printf("Cannot open the File LexicalAnalysis\n");
                exit(-1);
            }
            lexicalAnalysis();
            fclose(fp);
        }
        else
            lexicalAnalysis();
    }
    if (s)
    {
        if (f)
        {
            sprintf(filename, "%s", *p);
            fp = freopen(strcat(filename, ".ast"), "w", stdout);
            if (fp == NULL)
            {
                printf("Cannot open the File SyntaxAnalysis\n");
                exit(-1);
            }
            syntaxAnalysis();
            displayAST();
            fclose(fp);
        }
        else
        {
            syntaxAnalysis();
            displayAST();
        }
    }
    if (r)
    {
        if (s == 0)
            syntaxAnalysis();
        if (f)
        {
            sprintf(filename, "%s", *p);
            fp = freopen(strcat(filename, ".res"), "w", stdout);
            if (fp == NULL)
            {
                printf("Cannot open the File LexicalAnalysis\n");
                exit(-1);
            }
            rearrange();
            fclose(fp);
        }
        else
            rearrange();
    }

    freeAll();

    return 0;
}