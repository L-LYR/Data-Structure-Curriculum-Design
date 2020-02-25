#include <stdio.h>
#include <stdlib.h>
#include "global.h"
// int main(int argc, char **argv)
extern char *filename;
extern int token;

int main(void)
{
    FILE *fp;

    filename = "input.c";
    allocMemoryPool();
    readFromFile();

    fp = freopen("LexicalAnalysis", "w", stdout);
    if (fp == NULL)
    {
        printf("Cannot open the File LexicalAnalysis\n");
        exit(-1);
    }
    lexicalAnalysis();
    fclose(fp);

    fp = freopen("SyntaxAnalysis", "w", stdout);
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

    return 0;
}