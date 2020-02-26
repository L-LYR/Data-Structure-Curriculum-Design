#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

extern long long *symbols; // tokenizer.c
extern char *src, *oldSrc, *data, *oldData;

extern Node *ast; // ast.c

int poolSize, // size of memory pool
    line,     // current line in source code
    nodeNum;
char filename[64]; // source file name
// initialize memory pool
void allocMemoryPool()
{
    // fixed-size memory allocation
    poolSize = 102400;
    nodeNum = 10240;
    line = 1;

    if (!(symbols = malloc(poolSize)))
    {
        printf("Could not allocate %d memory for symbol table!\n", poolSize);
        exit(-1);
    }
    memset(symbols, 0, poolSize);

    if (!(oldData = data = malloc(poolSize)))
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
void freeExprNode(ExprNode *p);
void freeFunCallNode(FunCallNode *p)
{
    int i = 0;
    while (p->pl[i] != NULL)
    {
        freeExprNode(p->pl[i]);
        i++;
    }
}
void freeExprNode(ExprNode *p)
{
    if (p->t == PreUnary || p->t == Postfix)
    {
        freeExprNode(((UnaryOpNode *)(p->n))->o);
        free(p->n);
    }
    else if (p->t == Binary || p->t == Bracket)
    {
        freeExprNode(((BinaryOpNode *)(p->n))->lo);
        freeExprNode(((BinaryOpNode *)(p->n))->ro);
        free(p->n);
    }
    else if (p->t == Ternary)
    {
        freeExprNode(((TernaryOpNode *)(p->n))->c);
        freeExprNode(((TernaryOpNode *)(p->n))->a);
        freeExprNode(((TernaryOpNode *)(p->n))->b);
        free(p->n);
    }
    else if (p->t == FunCall)
    {
        freeFunCallNode(p->n);
        free(p->n);
    }
    else if (p->t == Variable)
    {
        free(p->n);
    }
    free(p);
}

void freeStatement(StateNode *p);
void freeBlockNode(BlockNode *p)
{
    StateNode *sl;
    sl = p->sl;
    while (sl->t != 0)
    {
        freeStatement(sl);
        sl++;
    }
}

void freeStatement(StateNode *p)
{
    if (p->t == IF)
    {
        freeExprNode(((IfNode *)(p->n))->c);
        freeStatement(&(((IfNode *)(p->n))->a));
        if (((IfNode *)(p->n))->b.t != EMPTY)
            freeStatement(&(((IfNode *)(p->n))->b));
        free(p->n);
    }
    else if (p->t == WHILE)
    {
        freeExprNode(((WhileNode *)(p->n))->c);
        freeStatement(&(((WhileNode *)(p->n))->s));
        free(p->n);
    }
    else if (p->t == BLOCK)
    {
        freeBlockNode(p->n);
        free(p->n);
    }
    else if (p->t == BREAK || p->t == CONTINUE || p->t == EMPTY)
    {
        free(p->n);
    }
    else if (p->t == RETURN)
    {
        if (p->n != NULL)
            freeExprNode(p->n);
    }
    else if (p->t == EXPR)
    {
        freeExprNode(p->n);
    }
}
void freeFuncBodyNode(Node *fb)
{
    Node *p = fb;
    while (p != NULL && p->t != 0)
    {
        if (p->t == LocDec)
            free(p->n);
        else if (p->t == Statement)
        {
            freeStatement(p->n);
            free(p->n);
        }
        p = p->s;
    }
}

void freeAST()
{
    Node *p = ast;
    while (p != NULL && p->t != 0)
    {
        if (p->t == EnumDec)
            free((EnumDecNode *)(p->n));
        else if (p->t == GloDec)
            free((GloDecNode *)(p->n));
        else if (p->t == FuncDef)
        {
            freeFuncBodyNode(((FuncDefNode *)(p->n))->fb);
            free(p->n);
        }
        p = p->s;
    }
    free(ast);
}

void freeAll()
{
    free(oldSrc);
    free(symbols);
    free(oldData);
    freeAST();
}