#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "enumVar.h"
#include "ast.h"

extern int line; // memoryPool.h

Node *ast, *lastNode, *curNode, *funcMark;

void *makeNode(int t)
{
    void *ret;

    if (t == EnumDec)
        ret = calloc(1, sizeof(EnumDecNode));
    else if (t == GloDec)
        ret = calloc(1, sizeof(GloDecNode));
    else if (t == FuncDef)
        ret = calloc(1, sizeof(FuncDefNode));
    else if (t == LocDec)
        ret = calloc(1, sizeof(LocDecNode));
    else if (t == Statement)
        ret = calloc(1, sizeof(StateNode));
    else if (t == IF)
        ret = calloc(1, sizeof(IfNode));
    else if (t == WHILE)
        ret = calloc(1, sizeof(WhileNode));
    else if (t == BLOCK)
        ret = calloc(1, sizeof(BlockNode));
    else if (t == RETURN || t == EXPR)
        ret = calloc(1, sizeof(ExprNode));
    else if (t == Variable)
        ret = calloc(1, sizeof(VarNode));
    else if (t == FunCall)
        ret = calloc(1, sizeof(FunCallNode));
    else if (t == Ternary)
        ret = calloc(1, sizeof(TernaryOpNode));
    else if (t == Binary || t == Bracket)
        ret = calloc(1, sizeof(BinaryOpNode));
    else if (t == PreUnary || t == Postfix)
        ret = calloc(1, sizeof(UnaryOpNode));

    if (ret == NULL)
    {
        printf("Cannot allocate memory for AST node\n");
        exit(-1);
    }

    return ret;
}
// global
void *setNode(int t)
{
    void *ret;

    curNode->t = t;
    ret = curNode->n = makeNode(t);
    curNode->l = line;

    if (lastNode != NULL)
    {
        if (lastNode->t == FuncDef)
            ((FuncDefNode *)(lastNode->n))->fb = curNode;
        else if (funcMark != NULL && (t == EnumDec || t == GloDec))
        {
            funcMark->s = curNode;
            funcMark = NULL;
        }
        else
            lastNode->s = curNode;
    }

    lastNode = curNode;
    curNode++;

    return ret;
}

void changeToFuncDef()
{
    GloDecNode *f;
    f = lastNode->n;
    lastNode->t = FuncDef;
    lastNode->n = makeNode(FuncDef);
    ((FuncDefNode *)lastNode->n)->f = f->fl[0];
    funcMark = lastNode;
    free(f);
}