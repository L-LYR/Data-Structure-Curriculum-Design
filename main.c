#include <stdio.h>
#include "global.h"
// int main(int argc, char **argv)
extern char *filename;
extern int token;

int main(void)
{
    filename = "input";
    allocMemoryPool();
    readFromFile();
    lexicalAnalysis();

    // syntaxAnalysis();
    return 0;
}