#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "enumVar.h"

#define ListLen 1024
#define NameLen 32

extern int token; // tokenizer.c
extern int *currentId;
extern int baseType; // parser.c

int *ast;
int *curNode, *lastNode;
int *l;

int makeList()
{
    int *p;
    if ((p = malloc(ListLen)) == NULL)
    {
        printf("Could not allocate memory for AST list branch\n");
        exit(-1);
    }
    memset(p, 0, ListLen);
    return (int)p;
}

static void EnumDecNode()
{
    curNode[nodetype] = EnumDec;
    curNode[base] = Num;
    curNode[nodelist] = makeList();
    lastNode = curNode;
    curNode = curNode + nodesize;
}
static void GloDecNode()
{
    curNode[nodetype] = GloDec;
    curNode[base] = baseType;
    curNode[nodelist] = makeList();
    lastNode = curNode;
    curNode = curNode + nodesize;
}

void DecNode()
{
    if (token == Enum)
        EnumDecNode();
    else if (token == Int || token == Char)
        GloDecNode();
}

void TransToFunDef()
{
    lastNode[nodetype] = FunDef;
    lastNode[nodename] = l[varName];
    lastNode[nodelist] = l[paralist];
    memset(l, 0, ListLen);
    lastNode[body] = (int)l;
}

static void printStar(int i)
{
    while (i > 0)
    {
        printf("*");
        i -= PTR;
    }
    printf("\n");
}
static void printType(int k)
{
    int i = 0;
    while (k >= BASE)
    {
        k -= PTR;
        i += PTR;
    }
    if (k == INT)
        printf("int");
    else if (k == CHAR)
        printf("char");
    printStar(i);
}

static void printParaList(int *t)
{
    char *n;
    int *p, i;
    while (*t)
    {
        p = (int *)t[paraName];
        n = (char *)p[Name];
        i = p[Len];
        printf("        Parameter Name: %.*s\n", i, n);
        printf("        Parameter Type: ");
        printType(t[paraType]);

        t = t + ParaNodeSize;
    }
}

static void displayEnumDec(int *p)
{
    int *q, *t, i;
    char *n;
    printf("Enum Declaration:\n    Enum Type Name: ");
    if (p[nodename] == 0)
        printf("%s\n", "(null)");
    else
    {
        t = (int *)p[nodename];
        i = t[Len];
        n = (char *)t[Name];
        printf("%.*s\n", i, n);
    }
    printf("    Variable Name<Value>: ");
    q = (int *)p[nodelist];
    while (*q != 0)
    {
        t = (int *)(*q++);
        i = t[Len];
        n = (char *)t[Name];
        printf("%.*s<%d> ", i, n, t[Value]);
    }
    printf("\n\n");
}

static void displayGloDec(int *p)
{
    int *q, *t, i;
    char *s, *n;
    if (p[base] == INT)
        s = "int";
    else if (p[base] == CHAR)
        s = "char";
    printf("Global Declaration:\n    Base Type: %s\n", s);
    q = (int *)p[nodelist];
    while (q[varName] != 0)
    {
        if (q[paralist] != 0)
        {
            t = (int *)q[varName];
            i = t[Len];
            n = (char *)t[Name];
            printf("    Function Name: %.*s\n", i, n);
            printf("    Return Type: %s ", s);
            printStar(q[ptr]);
            if (*(int *)q[paralist] == 0)
                printf("        No parameter\n");
            else
            {
                printf("    Parameter List: \n");
                printParaList((int *)q[paralist]);
            }
        }
        else
        {
            t = (int *)q[varName];
            i = t[Len];
            n = (char *)t[Name];
            printf("    Variable Name: %.*s\n", i, n);
            printf("    Variable Type: %s ", s);
            printStar(q[ptr]);
        }
        q = q + GloDecNodeSize;
    }
    printf("\n");
}

void display()
{
    int *p, *q, i, *t;
    char *s, *n;

    p = ast;
    while (*p)
    {
        if (p[nodetype] == EnumDec)
            displayEnumDec(p);
        else if (p[nodetype] == GloDec)
            displayGloDec(p);
        p += nodesize;
    }
}