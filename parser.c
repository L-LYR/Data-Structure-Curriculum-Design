#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enumVar.h"
#include "ast.h"

extern int line, poolSize; // memoryPool.c

extern char *src, *data; // tokenizer.c
extern int *currentId, *symbols, token, tokenVal;
extern void match(int tk);
extern int nextToken();

extern void initSymbolTab(); // lexer.c

extern Node *ast, *curNode; // ast.c
extern void *setNode(int t);

void *cn;
Var *vl;
Func *fl;

int baseType; // the type of a declaration
int exprType; // the type of an expression

// for virtual machine
int idxOfBp; // point to the entry of current function

static void enumDeclaration()
{
    int i;

    i = 0;

    vl = ((EnumDecNode *)cn)->vl;

    while (token != '}')
    {
        if (token != Id)
        {
            printf("Line %d: bad enum identifier\n", line, token);
            exit(-1);
        }

        if (currentId[Class])
        {
            // redeclaration
            if (currentId[Class] == Glo)
                printf("Line %d: redeclaration of deferent type\n", line);
            else if (currentId[Class] == Num)
                printf("Line %d: redeclaration of enumerator\n", line);
            else if (currentId[Class] == Fun)
                printf("Line %d: redeclaration of deferent type\n", line);
            exit(-1);
        }

        nextToken();

        if (token == Assign) // ENUM_VAR = 100
        {
            nextToken();
            if (token != Num)
            {
                printf("Line %d: bad enum initializer\n", line);
                exit(-1);
            }
            i = tokenVal;
            nextToken();
        }
        currentId[Class] = Num;
        currentId[Type] = INT;
        currentId[Value] = i++;

        memcpy(vl->id, (char *)currentId[Name], currentId[Len]);
        vl->t = currentId[Value];
        vl++;

        if (token == ',')
            nextToken();
    }
}

static void functionParameter()
{
    int type;
    int params;
    Var *p = fl->pl;
    params = 0;
    while (token != ')')
    {
        type = INT;
        if (token == Int) // int type
            match(Int);
        else if (token == Char) // char type
        {
            type = CHAR;
            match(Char);
        }
        else if (token == Void)
        {
            type = VOID;
            nextToken();
            return;
        }

        while (token == Mul) // pointer type
        {
            match(Mul);
            type = type + PTR;
        }

        if (token != Id)
        {
            printf("Line %d: bad parameter declaration\n", line);
            exit(-1);
        }
        if (currentId[Class])
        {
            if (currentId[Class] == Loc)
                printf("Line %d: duplicate parameter declaration\n", line);
            else if (currentId[Class] == Fun)
                printf("Line %d: redeclaration of function\n", line);
            else if (currentId[Class] == Num)
                printf("Line %d: redeclaration of deferent type\n", line);
            exit(-1);
        }

        match(Id);
        currentId[BClass] = currentId[Class];
        currentId[Class] = Loc;
        currentId[BType] = currentId[Type];
        currentId[Type] = type;
        currentId[BValue] = currentId[Value];
        currentId[Value] = params++;
        memcpy(p->id, (char *)currentId[Name], currentId[Len]);
        p->t = type;
        p++;
        if (token == ',')
            match(',');
    }
    idxOfBp = params + 1;
}

static void functionDeclaration()
{

    // Example: Function Name (Parameter List) {Function Body}
    match('(');
    functionParameter();
    match(')');

    // TODO: distinguish the function definition and function declaration
    // match('{');
    // functionBody();
    // Here we do not need to match '}'.
    // It will be matched in globalDeclaration() to jump out the while().

    // exit function
    // cover the local variables in function body with global variables which have the same name

    currentId = symbols;
    while (currentId[Token])
    {
        if (currentId[Class] == Loc)
        {
            currentId[Class] = currentId[BClass];
            currentId[Type] = currentId[BType];
            currentId[Value] = currentId[BValue];
        }
        currentId = currentId + IdSize;
    }
}

static void globalDeclaration()
{
    int type;
    int i;

    baseType = INT; // default type

    if (token == Enum)
    {
        cn = setNode(EnumDec);

        match(Enum);
        if (token != '{')
        {
            match(Id);
            memcpy(((EnumDecNode *)cn)->id, (char *)currentId[Name], currentId[Len]);
        }

        if (token == '{')
        {
            match('{');
            enumDeclaration();
            match('}');
        }
        match(';');
        return;
    }

    // parse type name
    // Example: int | char [*]
    if (token == Int)
        match(Int);
    else if (token == Char)
    {
        baseType = CHAR;
        match(Char);
    }
    else if (token == Void)
    {
        baseType = VOID;
        match(Void);
    }

    cn = setNode(GloDec);

    vl = ((GloDecNode *)cn)->vl;
    fl = ((GloDecNode *)cn)->fl;

    // parse the comma seperated variable declaration
    // Example: int [*] a, b, ... ;
    while (token != ';' && token != '}')
    {
        type = baseType;

        while (token == Mul)
        {
            match(Mul);
            type = type + PTR;
        }
        if (token != Id)
        {
            // invalid declaration
            printf("Line %d: bad global declaration\n", line);
            exit(-1);
        }

        // redeclaration
        if (currentId[Class] == Num)
        {
            printf("Line %d: redeclaration of deferent type\n", line);
            exit(-1);
        }
        if (currentId[Class] == Glo)
        {
            printf("Line %d: redeclaration of global variable\n", line);
            exit(-1);
        }

        match(Id);
        currentId[Type] = type;
        if (token == '(')
        {
            currentId[Class] = Fun;
            memcpy(fl->id, (char *)currentId[Name], currentId[Len]);
            fl->rt = type;
            // currentId[Value] = (int)(text + 1); // memory address of function or entry
            functionDeclaration();
            fl++;
        }
        else
        {
            if (type == VOID)
            {
                printf("Line %d: variable cannot have void type\n", line);
                exit(-1);
            }
            currentId[Class] = Glo; // global variable
            // currentId[Value] = (int)data; // assign memory
            // data = data + sizeof(int);

            memcpy(vl->id, (char *)currentId[Name], currentId[Len]);
            vl->t = type;
            vl++;
        }
        if (token == ',')
            match(',');
    }
    nextToken();
}

void syntaxAnalysis()
{
    initSymbolTab();
    curNode = ast;

    nextToken();
    while (token > 0)
        globalDeclaration();
}