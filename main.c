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
    f = l = s = r = 0;
    argc--;
    argv++;

    if (argc < 1 || argc > 5)
    {
        printf("usage: mlpc <-operation> <filename> \n");
        printf("    -l  lexical analysis\n");
        printf("    -s  syntax analysis\n");
        printf("    -r  rearrange source code\n");
        printf("    -f  save all outputs in files\n");
        return -1;
    }

    while (argv > 0 && **argv == '-')
    {
        switch ((*argv)[1])
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
        }
        --argc;
        ++argv;
    }

    if (argc < 1)
    {
        printf("usage: mlpc <-operation> <filename> \n");
        printf("    -l  lexical analysis\n");
        printf("    -s  syntax analysis\n");
        printf("    -r  rearrange source code\n");
        printf("    -f  save all outputs in files\n");
        return -1;
    }

    sprintf(filename, "%s", *argv);
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
            sprintf(filename, "%s", *argv);
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
            sprintf(filename, "%s", *argv);
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