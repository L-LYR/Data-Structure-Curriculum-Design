#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include <string.h>
// int main(int argc, char **argv)
extern char filename[64];

int main(void)
{
    FILE *fp;

    sprintf(filename, "%s", "input.c");
    allocMemoryPool();
    readFromFile();

    fp = freopen(strcat(filename, ".lex"), "w", stdout);
    if (fp == NULL)
    {
        printf("Cannot open the File LexicalAnalysis\n");
        exit(-1);
    }
    lexicalAnalysis();
    fclose(fp);

    sprintf(filename, "%s", "input.c");
    fp = freopen(strcat(filename, ".ast"), "w", stdout);
    if (fp == NULL)
    {
        printf("Cannot open the File SyntaxAnalysis\n");
        exit(-1);
    }
    syntaxAnalysis();
    displayAST();
    fclose(fp);
    fp = freopen("output.c", "w", stdout);
    if (fp == NULL)
    {
        printf("Cannot open the File LexicalAnalysis\n");
        exit(-1);
    }
    rearrange();
    fclose(fp);

    freeAll();

    return 0;
}