#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "enumVar.h"

extern char *map[]; // lexer.c
extern Node *ast;   // ast.c
extern void indent(),
    displayStar(int i),
    displayType(int t); //display.c
extern int ind;

int flag;

static void printEnumDec(EnumDecNode *p)
{
    Var *q;
    int i = 0;
    printf("enum");
    if (*(p->id))
        printf(" %s\n", p->id);
    printf("{\n");
    ind += 4;
    q = p->vl;
    while (*(q->id))
    {
        indent();
        printf("%s", q->id);
        if (i != q->t)
        {
            printf(" = %d", q->t);
            i = q->t;
        }
        printf(",\n");
        i++;
        q++;
    }
    ind -= 4;
    printf("};\n");
}

static void printGloDec(GloDecNode *p)
{
    int i;
    Var *q;
    Func *f;

    f = p->fl;
    q = p->vl;

    flag = 0;
    if (*(q->id))
    {
        i = q->t % PTR;
        printf("%s", map[i]);
        while (*(q->id))
        {
            if (flag == 0)
                flag = 1;
            else
                putchar(',');
            putchar(' ');

            displayStar(q->t / PTR);
            printf("%s", q->id);
            q++;
        }
        printf(";\n");
    }

    if (*(f->id))
    {
        while (*(f->id))
        {
            displayType(f->rt);
            printf("%s(", f->id);
            flag = 0;
            q = f->pl;
            while (*(q->id))
            {
                if (flag == 0)
                    flag = 1;
                else
                    printf(", ");
                displayType(q->t);
                printf("%s", q->id);
                q++;
            }
            printf(");\n");
            f++;
        }
    }
}
static void printLocDec(LocDecNode *p)
{
    indent();
    int i;
    Var *q;

    q = p->vl;
    if (*(q->id))
    {
        i = q->t % PTR;
        printf("%s", map[i]);
        flag = 0;
        while (*(q->id))
        {
            if (flag == 0)
                flag = 1;
            else
                putchar(',');
            putchar(' ');

            displayStar(q->t / PTR);
            printf("%s", q->id);
            q++;
        }
        printf(";\n");
    }
}
static void printExpr(ExprNode *p);
static void printFunCall(FunCallNode *p)
{
    int i = 0;
    printf("%s(", p->n);
    flag = 0;
    while (p->pl[i] != NULL)
    {
        if (flag == 0)
            flag = 1;
        else
            printf(", ");
        printExpr(p->pl[i]);
        i++;
    }
    putchar(')');
}

static void printUnaryOp(UnaryOpNode *p, int t)
{
    if (p->op[0] == '(')
    {
        putchar('(');
        printExpr(p->o);
        putchar(')');
        return;
    }

    if (p->op[1] == 1)
    {
        putchar('(');
        displayType(p->op[0]);
        putchar(')');
    }
    else if (t == PreUnary)
        printf("%s", p->op);
    printExpr(p->o);
    if (t == Postfix)
        printf("%s", p->op);
}

static void printBinaryOp(BinaryOpNode *p, int t)
{
    printExpr(p->lo);
    if (t == Binary)
        printf(" %s ", p->op);
    else if (t == Bracket)
        putchar('[');
    printExpr(p->ro);
    if (t == Bracket)
        putchar(']');
}

static void printTernaryOp(TernaryOpNode *p)
{
    printExpr(p->c);
    printf(" ? ");
    printExpr(p->a);
    printf(" : ");
    printExpr(p->b);
}

static void printExpr(ExprNode *p)
{
    if (p->t == ConstInt)
        printf("%lld", (long long)(p->n));
    else if (p->t == ConstFlo)
        printf("%s", (char *)(p->n));
    else if (p->t == ConstStr)
        printf("\"%s\"", (char *)(p->n));
    else if (p->t == FunCall)
        printFunCall(p->n);
    else if (p->t == Variable)
        printf("%s", ((VarNode *)(p->n))->n);
    else if (p->t == PreUnary || p->t == Postfix)
        printUnaryOp(p->n, p->t);
    else if (p->t == Binary || p->t == Bracket)
        printBinaryOp(p->n, p->t);
    else if (p->t == Ternary)
        printTernaryOp(p->n);
}
static void printStatement(StateNode *p);
static void printIfStm(IfNode *p)
{
    printf("if (");
    printExpr(p->c);
    printf(")\n");
    printStatement(&(p->a));
    if (p->b.t != EMPTY)
    {
        indent();
        printf("else\n");
        printStatement(&(p->b));
    }
}

static void printWhileStm(WhileNode *p)
{
    printf("while (");
    printExpr(p->c);
    printf(")\n");
    printStatement(&(p->s));
}

static void printBlockStm(BlockNode *p)
{
    StateNode *sl;
    printf("{\n");
    ind += 4;
    sl = p->sl;
    if (sl->t != 0)
    {
        while (sl->t != 0)
        {
            printStatement(sl);
            sl++;
        }
    }
    ind -= 4;
    indent();
    printf("}\n");
}

static void printStatement(StateNode *p)
{
    indent();
    if (p->t == IF)
        printIfStm(p->n);
    else if (p->t == WHILE)
        printWhileStm(p->n);
    else if (p->t == BLOCK)
        printBlockStm(p->n);
    else if (p->t == BREAK)
        printf("break;\n");
    else if (p->t == CONTINUE)
        printf("continue;\n");
    else if (p->t == RETURN)
    {
        printf("return ");
        if (p->n != NULL)
            printExpr(p->n);
        printf(";\n");
    }
    else if (p->t == EXPR)
    {
        printExpr(p->n);
        printf(";\n");
    }
    else if (p->t == EMPTY)
        printf(";\n");
}

static void printFuncBody(Node *fb)
{
    ind += 4;
    Node *p = fb;
    while (p != NULL && p->t != 0)
    {
        if (p->t == LocDec)
            printLocDec(p->n);
        if (p->t == Statement)
            printStatement(p->n);
        p = p->s;
    }
    ind -= 4;
}

static void printFuncDef(FuncDefNode *p)
{
    Var *q;
    displayType(p->f.rt);
    printf("%s(", p->f.id);
    q = p->f.pl;
    flag = 0;
    while (*(q->id))
    {
        if (flag == 0)
            flag = 1;
        else
            printf(", ");
        displayType(q->t);
        printf("%s", q->id);
        q++;
    }
    printf(")\n{\n");
    printFuncBody(p->fb);
    printf("}\n");
}

void rearrange()
{
    ind = 0;
    Node *p = ast;
    while (p != NULL && p->t != 0)
    {
        if (p->t == EnumDec)
            printEnumDec(p->n);
        else if (p->t == GloDec)
            printGloDec(p->n);
        else if (p->t == FuncDef)
            printFuncDef(p->n);
        p = p->s;
    }
}