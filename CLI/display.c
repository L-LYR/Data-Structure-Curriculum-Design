#include <stdio.h>
#include "ast.h"
#include "enumVar.h"

extern Node *ast;

extern char *map[]; // lexer.c
int ind;

void indent()
{
    int i;
    i = 0;
    while (i++ < ind)
        putchar(' ');
}

static void displayEnumDecNode(EnumDecNode *p)
{
    Var *q;
    printf("Enum Declaration: \n");

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

void displayStar(int i)
{
    while (i-- > 0)
        putchar('*');
}

void displayType(int t)
{
    printf("%s ", map[t % PTR]);
    displayStar(t / PTR);
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
    printf("Global Declaration: \n");
    if (*((p->vl)->id))
    {
        ind += 4;
        indent();
        printf("Global Variable Declaration: \n");
        displayVarList(p->vl);
        ind -= 4;
    }

    if (*((p->fl)->id))
    {
        ind += 4;
        indent();
        printf("Function Declaration: \n");
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
            printf("Parameter List: ");
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

static void displayLocDecNode(LocDecNode *p)
{
    printf("Line %d: \n", p->l);
    indent();
    printf("Local Variable Declaration: \n");
    displayVarList(p->vl);
}
static void displayExpr(ExprNode *p);
static void displayFunCall(FunCallNode *p)
{
    int i = 0;
    ind += 4;
    indent();
    printf("Function Name: %s\n", p->n);
    indent();
    printf("Input Parameter List: \n");
    ind += 4;
    while (p->pl[i] != NULL)
    {
        displayExpr(p->pl[i]);
        i++;
    }
    ind -= 8;
}

static void displayVarExpr(VarNode *p)
{
    printf("Primary Expression: %s\n", p->n);
    ind += 4;
    indent();
    printf("Type: ");
    displayType(p->t);
    printf("\n");
    indent();
    if (p->r == Num)
        printf("Enum Variable Value: %d\n", p->t);
    else
    {
        printf("Variable Range: ");
        if (p->r == Glo)
            printf("Global\n");
        else if (p->r == Loc)
            printf("Local\n");
    }
    ind -= 4;
}

static void displayUnaryOp(UnaryOpNode *p, int t)
{
    if (p->op[0] == '(')
    {
        printf("Parenthesis: %s\n", p->op);
        indent();
        printf("Content: \n");
        ind += 4;
        displayExpr(p->o);
        ind -= 4;
        return;
    }

    if (p->op[1] == 1)
    {
        printf("Cast Expression: (");
        displayType(p->op[0]);
        printf(")\n");
    }
    else if (t == PreUnary)
        printf("Unary Expression: %s\n", p->op);
    else if (t == Postfix)
        printf("Postfix Expression: %s\n", p->op);
    indent();
    printf("Operand: \n");
    ind += 4;
    displayExpr(p->o);
    ind -= 4;
}

static void displayBinaryOp(BinaryOpNode *p, int t)
{
    if (t == Binary)
        printf("Binary Expression: %s\n", p->op);
    else if (t == Bracket)
        printf("PostFix Expression: []\n");

    indent();
    printf("Left Operand: \n");
    ind += 4;
    displayExpr(p->lo);
    ind -= 4;

    indent();
    printf("Right Operand: \n");
    ind += 4;
    displayExpr(p->ro);
    ind -= 4;
}

static void displayTernaryOp(TernaryOpNode *p)
{
    printf("Ternary Expression: ? :\n");

    indent();
    printf("Condition: \n");
    ind += 4;
    displayExpr(p->c);
    ind -= 4;

    indent();
    printf("Branch A: \n");
    ind += 4;
    displayExpr(p->a);
    ind -= 4;

    indent();
    printf("Branch B: \n");
    ind += 4;
    displayExpr(p->b);
    ind -= 4;
}
static void displayExpr(ExprNode *p)
{
    indent();
    if (p->t == ConstInt)
        printf("Constant Expression: %lld\n", (long long)(p->n));
    else if (p->t == ConstFlo)
        printf("Constant Expression: %s\n", (char *)(p->n));
    else if (p->t == ConstStr)
        printf("Constant Expression: \"%s\"\n", (char *)(p->n));
    else if (p->t == FunCall)
    {
        printf("Function Call:\n");
        displayFunCall(p->n);
    }
    else if (p->t == Variable)
        displayVarExpr(p->n);
    else if (p->t == PreUnary || p->t == Postfix)
        displayUnaryOp(p->n, p->t);
    else if (p->t == Binary || p->t == Bracket)
        displayBinaryOp(p->n, p->t);
    else if (p->t == Ternary)
        displayTernaryOp(p->n);
}

static void displayStatement(StateNode *p);
static void displayIfStm(IfNode *p)
{
    printf("If Statement: \n");

    ind += 4;
    indent();
    printf("Condition: \n");
    ind += 4;
    displayExpr(p->c);

    ind -= 4;
    indent();
    printf("Branch A: \n");
    ind += 4;
    displayStatement(&(p->a));
    ind -= 4;

    if (p->b.t != EMPTY)
    {
        indent();
        printf("Branch B: \n");
        ind += 4;
        displayStatement(&(p->b));
        ind -= 4;
    }

    ind -= 4;
}

static void displayWhileStm(WhileNode *p)
{
    printf("While Statement: \n");

    ind += 4;
    indent();
    printf("Condition: \n");
    ind += 4;
    displayExpr(p->c);

    ind -= 4;
    indent();
    printf("Loop Body: \n");
    ind += 4;
    displayStatement(&(p->s));
    ind -= 8;
}

static void displayBlockStm(BlockNode *p)
{
    StateNode *sl;
    printf("Block Statement: ");
    ind += 4;
    sl = p->sl;

    if (!(sl->t))
        printf("(null)\n");
    else
    {
        printf("\n");
        while (sl->t)
        {
            displayStatement(sl);
            sl++;
        }
    }
    ind -= 4;
}

static void displayStatement(StateNode *p)
{
    printf("Line %d: \n", p->l);
    indent();
    if (p->t == IF)
        displayIfStm(p->n);
    else if (p->t == WHILE)
        displayWhileStm(p->n);
    else if (p->t == BLOCK)
        displayBlockStm(p->n);
    else if (p->t == BREAK)
        printf("Jump Statement: break\n");
    else if (p->t == CONTINUE)
        printf("Jump Statement: continue\n");
    else if (p->t == RETURN)
    {
        printf("Jump Statement: return\n");
        ind += 4;
        if (p->n != NULL)
            displayExpr(p->n);
        else
        {
            indent();
            printf("Empty Statement: (null)\n");
        }
        ind -= 4;
    }
    else if (p->t == EXPR)
    {
        printf("One Line Statement: \n");
        ind += 4;
        displayExpr(p->n);
        ind -= 4;
    }
    else if (p->t == EMPTY)
        printf("Empty Statement: (null)\n");
}

static void displayFuncBody(Node *fb)
{
    ind += 4;
    Node *p = fb;
    while (p != NULL && p->t != 0)
    {
        if (p->t == LocDec)
            displayLocDecNode(p->n);
        if (p->t == Statement)
            displayStatement(p->n);
        p = p->s;
    }
    ind -= 4;
}

static void displayFuncDef(FuncDefNode *p)
{
    printf("Function Definition: \n");

    ind += 4;
    indent();
    printf("Function Name: %s\n", p->f.id);

    indent();
    printf("Return Type: ");
    displayType(p->f.rt);
    printf("\n");

    indent();
    printf("Parameter List: ");
    if (*(p->f.pl)->id)
    {
        printf("\n");
        displayVarList(p->f.pl);
    }
    else
        printf("(null)\n");

    indent();
    printf("Function Body: \n");
    displayFuncBody(p->fb);
    ind -= 4;
}

void displayAST()
{
    Node *p = ast;
    while (p != NULL && p->t != 0)
    {
        printf("Line %d: \n", p->l);
        if (p->t == EnumDec)
            displayEnumDecNode(p->n);
        else if (p->t == GloDec)
            displayGloDecNode(p->n);
        else if (p->t == FuncDef)
            displayFuncDef(p->n);
        printf("\n");
        p = p->s;
    }
}