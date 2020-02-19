#ifndef AST_H
#define AST_H

#define nameLen 32
#define listLen 16

enum NodeType
{
    EnumDec = 512,
    GloDec,
    // TODO: statements
    FunDef,

    LocDec,

    statement,
    UnaryExpr,
    BinaryExpr,
    FunCall,
    CondExpr,
    UnitExpr, // only for single Variable, String, Num, Empty.
};

typedef struct Var
{
    char id[nameLen];
    int t; // type or value
} Var;

typedef struct Func
{
    char id[nameLen];
    int rt;                 // return type
    struct Var pl[listLen]; // parameter list
    struct Node *fb;        // function body
} Func;

typedef struct EnumDecNode
{
    char id[nameLen];
    struct Var vl[listLen]; // variable list
} EnumDecNode;

typedef struct GloDecNode
{
    struct Var vl[listLen];  // variable list
    struct Func fl[listLen]; // function list
} GloDecNode;

typedef struct Node
{
    int t;          // type
    void *n;        // node
    struct Node *s; //successor
} Node;

#endif