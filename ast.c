#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "enumVar.h"
#include "ast.h"

Node *ast, *lastNode, *curNode;

static void *makeNode(int t)
{
    void *ret;

    if (t == EnumDec)
        ret = calloc(1, sizeof(EnumDecNode));
    else if (t == GloDec)
        ret = calloc(1, sizeof(GloDecNode));

    if (ret == NULL)
    {
        printf("Cannot allocate memory for AST node\n");
        exit(-1);
    }

    return ret;
}

void *setNode(int t)
{
    void *ret;

    curNode->t = t;
    ret = curNode->n = makeNode(t);

    if (lastNode != NULL)
        lastNode->s = curNode;
    lastNode = curNode;

    curNode++;

    return ret;
}