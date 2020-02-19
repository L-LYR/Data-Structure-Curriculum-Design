#include <stdio.h>
#include "ast.h"
#include "enumVar.h"

extern Node *ast;

char *typeMap[] = {"void", "char", "int"};
int ind;

static void indent()
{
    int i;
    i = 0;
    while (i++ < ind)
        putchar(' ');
}

static void displayEnumDecNode(EnumDecNode *p)
{
    Var *q;
    printf("Enum Declaration:\n");

    ind += 4;
    indent();
    printf("Enum Type Name: ");
    if (*(p->id))
        printf("%s\n", p->id);
    else
        printf("(null)\n");

    indent();
    printf("Enum Variable List: \n");
    ind += 4;
    q = p->vl;
    while (*(q->id))
    {
        indent();
        printf("%s <%d>\n", q->id, q->t);
        q++;
    }

    ind -= 8;
}

static void displayType(int t)
{
    int i;
    i = t / PTR;
    t = t % PTR;
    printf("%s ", typeMap[t]);
    while (i-- > 0)
        putchar('*');
}

static void displayVarList(Var *p)
{
    ind += 4;
    while (*(p->id))
    {
        indent();
        printf("Variable Name: %s\n", p->id);
        indent();
        printf("Variable Type: ");
        displayType(p->t);
        printf("\n");
        p++;
    }
    ind -= 4;
}

static void displayGloDecNode(GloDecNode *p)
{
    Func *f;
    printf("Global Declaraion:\n");
    if (*((p->vl)->id))
    {
        ind += 4;
        indent();
        printf("Global Variable Declaration:\n");
        displayVarList(p->vl);
        ind -= 4;
    }

    if (*((p->fl)->id))
    {
        ind += 4;
        indent();
        printf("Function Declaration:\n");
        ind += 4;
        f = p->fl;
        while (*(f->id))
        {
            indent();
            printf("Function Name: %s\n", f->id);

            indent();
            printf("Return Type: ");
            displayType(f->rt);
            printf("\n");

            indent();
            printf("Parameter List:");
            if (*(f->pl)->id)
            {
                printf("\n");
                displayVarList(f->pl);
            }
            else
                printf("(null)\n");
            f++;
        }
        ind -= 8;
    }
}

void displayAST()
{
    Node *p = ast;
    while (p != NULL)
    {
        if (p->t == EnumDec)
            displayEnumDecNode(p->n);
        else if (p->t == GloDec)
            displayGloDecNode(p->n);
        p = p->s;
        printf("\n");
    }
}