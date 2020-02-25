#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

extern int *symbols; // tokenizer.c
extern char *src, *oldSrc, *data;

extern Node *ast; // ast.c

int poolSize, // size of memory pool
    line,     // current line in source code
    nodeNum;
char *filename; // source file name
// initialize memory pool
void allocMemoryPool()
{
    // fixed-size memory allocation
    poolSize = 102400;
    nodeNum = 5120;
    line = 1;

    if (!(symbols = malloc(poolSize)))
    {
        printf("Could not allocate %d memory for symbol table!\n", poolSize);
        exit(-1);
    }
    memset(symbols, 0, poolSize);

    if (!(data = malloc(poolSize)))
    {
        printf("Could not allocate %d memory for data area!\n", poolSize);
        exit(-1);
    }
    memset(data, 0, poolSize);

    if (!(ast = malloc(nodeNum * sizeof(Node))))
    {
        printf("Could not allocate %d memory for AST data area!\n", nodeNum * sizeof(Node));
        exit(-1);
    }
    memset(ast, 0, sizeof(nodeNum * sizeof(Node)));
}
// create source file stream
void readFromFile()
{
    FILE *fp;
    int ret;
    if (!(fp = fopen(filename, "r")))
    {
        printf("Could not open file at %s!\n", filename);
        exit(-1);
    }
    if (!(src = oldSrc = malloc(poolSize)))
    {
        printf("Could not allocate %d memory for source code!\n", poolSize);
        exit(-1);
    }

    fseek(fp, 0L, SEEK_END);
    int fileSize = ftell(fp);
    rewind(fp);
    if ((ret = fread(src, 1, fileSize, fp)) < fileSize)
    {
        printf("File read error! Return %d and expect %d.\n", ret, fileSize);
        exit(-1);
    }
    fclose(fp);
}