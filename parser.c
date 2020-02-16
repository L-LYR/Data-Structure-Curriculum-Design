#include <stdio.h>
#include <stdlib.h>
#include "enumVar.h"
#include "stack.h"
extern int line;
extern char *src; // tokenizer.c
extern char *data;
extern int *currentId;
extern int *symbols;
extern int token;
extern int tokenVal;
extern void match(int tk);
extern int nextToken();
extern void initSymbolTab();

// STACK_TEMPLATE(int, IntStack); // for ast node
// int *ast,                      // root of AST
//     *curNode;

int baseType; // the type of a declaration
int exprType; // the type of an expression

// for virtual machine
int idxOfBp; // point to the entry of current function

static void enumDeclaration()
{
    int i;

    i = 0;

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

        if (token == ',')
            nextToken();
    }
    
}

static void functionParameter()
{
    int type;
    int params;

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
        if (currentId[Class] == Loc)
        {
            printf("Line %d: duplicate parameter declaration\n", line);
            exit(-1);
        }

        match(Id);
        currentId[BClass] = currentId[Class];
        currentId[Class] = Loc;
        currentId[BType] = currentId[Type];
        currentId[Type] = type;
        currentId[BValue] = currentId[Value];
        currentId[Value] = params++;
        if (token == ',')
            match(',');
    }
    idxOfBp = params + 1;
}
// forward declaration
static void parseExpression(int level);
static void unitUnary()
{
    int *id;
    int tmp;
    if (token == Num)
    {
        match(Num);
        exprType = INT;
    }
    else if (token == '"')
    {

        match('"');
        while (token == '"')
            match('"');

        // append the end of string character '\0', all the data are default
        // to 0, so just move data one position forward.
        data = (char *)(((int)data + sizeof(int)) & (-sizeof(int)));
        exprType = PTR;
    }
    else if (token == Sizeof)
    {
        match(Sizeof);
        match('(');
        exprType = INT;
        if (token == Int)
            match(Int);
        else if (token == Char)
        {
            match(Char);
            exprType = CHAR;
        }

        while (token == Mul)
        {
            match(Mul);
            exprType = exprType + PTR;
        }

        if (token == Id)
        {
            if (currentId[Type] == 0)
            {
                printf("Line %d: Undeclared variable\n", line);
                exit(-1);
            }
            exprType = currentId[Type];
        }

        if (token == Num)
            exprType = INT;

        match(')');

        // (exprType == CHAR) ? sizeof(char) : sizeof(int);

        exprType = INT;
    }
    else if (token == Id)
    {
        match(Id);
        id = currentId;
        if (token == '(')
        {
            if (id[Class] != Fun)
            {
                printf("Line %d: bad function call\n", line);
                exit(-1);
            }

            match('('); // function call;
            while (token != ')')
            {
                parseExpression(Assign);
                if (token == ',')
                    match(',');
            }

            match(')');

            exprType = id[Type];
        }
        else if (id[Class] == Num) // enum
        {
            exprType = INT;
        }
        else // variable
        {
            if (id[Class] == Loc)
            {
            }
            else if (id[Class] == Glo)
            {
            }
            else
            {
                printf("Line %d: Undeclared variable\n", line);
                exit(-1);
            }
        }
        exprType = id[Type];
    }
    else if (token == '(') // cast
    {
        match('(');
        if (token == Int || token == Char)
        {
            tmp = (token == Char) ? CHAR : INT;
            match(token);
            while (token == Mul)
            {
                match(Mul);
                tmp = tmp + PTR;
            }

            match(')');

            parseExpression(Inc);
            exprType = tmp;
        }
        else // normal
        {
            parseExpression(Assign);
            match(')');
        }
    }
    else if (token == Mul)
    {
        match('Mul');
        parseExpression(Inc);
        if (parseExpression >= BASE)
            exprType = exprType - PTR;
        else
        {
            printf("Line %d: bad dereference\n", line);
            exit(-1);
        }
    }
    else if (token == And)
    {
        match(And);
        parseExpression(Inc);
        // TODO: error?
        exprType = exprType + PTR;
    }
    else if (token == '!')
    {
        match('!');
        parseExpression(Inc);

        exprType = INT;
    }
    else if (token == '~')
    {
        match('~');
        parseExpression(Inc);

        exprType = INT;
    }
    else if (token == Sub)
    { // -value
        match(Sub);
        if (token == Num)
        {
            match(Num);
        }
        else
        {
            parseExpression(Inc);
        }

        exprType = INT;
    }
    else if (token == Inc || token == Dec)
    {
        tmp = token;
        match(token);
        parseExpression(Inc);

        // TODO: how to judge
    }
    else
    {
        printf("Line %d: bad parseExpression\n", line);
        exit(-1);
    }
}

static void binaryOperator(int level)
{
    int tmp;
    while (token >= level)
    {
        tmp = exprType;
        if (token == Assign)
        {
            match(Assign);
        }
    }
}
static void parseExpression(int level)
{
    if (token == 0)
    {
        printf("Line %d: unexpected token EOF of parseExpression\n", line);
        exit(-1);
    }
    unitUnary();
    binaryOperator(level);
}

static void parseStatement()
{
    // int *a, *b; // branches

    // Example: if(expression) <statement> [else <statement>]
    if (token == If)
    {
        match(If);
        match('(');
        parseExpression(Assign); // condition
        match(')');

        parseStatement();

        if (token == Else)
        {
            match(Else);
            parseStatement();
        }
    }
}

static void functionBody()
{
    int localPos;
    int type;
    localPos = idxOfBp;

    // declarations
    while (token == Int || token == Char)
    {
        baseType = (token == Int) ? INT : CHAR;
        match(token);

        while (token != ';')
        {
            type = baseType;
            while (token == Mul) // pointer
            {
                match(Mul);
                type = type + PTR;
            }
            if (token != Id)
            {
                printf("Line %d: bad local declaration\n", line);
                exit(-1);
            }
            if (currentId[Class] == Loc)
            {
                printf("Line %d: redeclaration of local variable\n", line);
                exit(-1);
            }

            match(Id);

            currentId[BClass] = currentId[Class];
            currentId[Class] = Loc;
            currentId[BType] = currentId[Type];
            currentId[Type] = type;
            currentId[BValue] = currentId[Value];
            currentId[Value] = ++localPos;

            if (token == ',')
                match(',');
        }
        match(';');
    }
    // TODO: handle the function node
    // *++text = ENT;
    // *++text = localPos - idxOfBp;

    while (token != '}')
        parseStatement();
    // TODO: handle the return statement
    // *++text = LEV;
}

static void functionDeclaration()
{
    // Example: Function Name (Parameter List) {Function Body}
    match('(');
    functionParameter();
    match(')');
    // TODO: distinguish the function definition and function declaration
    match('{');
    functionBody();
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
        match(Enum);
        if (token != '{')
            match(Id);
        else if (token == '{')
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
        match(Char);
        baseType = CHAR;
    }
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

        if (currentId[Class])
        {
            // redeclaration
            if (currentId[Class] == Num)
                printf("Line %d: redeclaration of deferent type\n", line);
            else if (currentId[Class] == Glo)
                printf("Line %d: redeclaration of global variable\n", line);
            exit(-1);
        }

        match(Id);
        currentId[Type] = type;
        if (token == '(')
        {
            currentId[Class] = Fun;
            // currentId[Value] = (int)(text + 1); // memory address of function or entry
            // TODO: here we need to create the node of function
            functionDeclaration();
        }
        else
        {
            currentId[Class] = Glo;       // global variable
            currentId[Value] = (int)data; // assign memory
            data = data + sizeof(int);
        }
        if (token == ',')
            match(',');
    }
    nextToken();
}

void syntaxAnalysis()
{
    initSymbolTab();
    nextToken();
    while (token > 0)
        globalDeclaration();
}