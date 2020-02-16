#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern int *symbols; // tokenizer.c
extern char *src;
extern char *oldSrc;
extern char *data;
extern int *ast;

int poolSize,   // size of memory pool
    line;       // current line in source code
char *filename; // source file name
// initialize memory pool
void allocMemoryPool()
{
    poolSize = 256 * 1024;
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

    if (!(ast = malloc(poolSize)))
    {
        printf("Could not allocate %d memory for creating AST!\n", poolSize);
        exit(-1);
    }
    memset(ast, 0, poolSize);
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