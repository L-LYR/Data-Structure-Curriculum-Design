#ifndef AST_H
#define AST_H
// muteable
#define nameLen 32
#define listLen 32

enum NodeType
{
    EnumDec = 512,
    GloDec,
    FuncDef,
    LocDec,
    Statement,
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
} Func;
// <enum> [id] <{> <id>[= Num][<,> <id>[= Num]] <}> <;>
typedef struct EnumDecNode
{
    char id[nameLen];
    struct Var vl[listLen]; // variable list
} EnumDecNode;

// <type> [*] <id1> [<(> <type> [*] <p1> [<,> <type> [*] <p2>] <)>] [<,> [*] <id2>] <;>
typedef struct GloDecNode
{
    struct Var vl[listLen];  // variable list
    struct Func fl[listLen]; // function list
} GloDecNode;

// <type> [*] <id1> <(> <type> [*] <p1> [<,> <type> [*] <p2>] <)> <{> [declaration] [statement] <}>
typedef struct FuncDefNode
{
    struct Func f;
    struct Node *fb; // function body
} FuncDefNode;

// <type> [*] <id1>  [<,> [*] <id2>] <;>
typedef struct LocDecNode
{
    struct Var vl[listLen];
    int l;
} LocDecNode;

enum ExprType
{
    PreUnary = 768,
    Postfix,
    Bracket,
    Binary,
    Ternary, // ? :
    FunCall,
    Variable,
    ConstInt,
    ConstFlo,
    ConstStr,
};
typedef struct ExprNode
{
    int t;   // type
    void *n; // node
} ExprNode;
// specific expression node
// <id>
typedef struct VarNode
{
    int r;           // range
    int t;           // type
    char n[nameLen]; // name
} VarNode;
// <op> <expression>
// <expression> <op>
typedef struct UnaryOpNode
{
    char op[3];         // operator
    struct ExprNode *o; // operand
} UnaryOpNode;
// <expression> <op> <expression>
typedef struct BinaryOpNode
{
    char op[3];          // operator
    struct ExprNode *lo; // left operand
    struct ExprNode *ro; // right operand
} BinaryOpNode;
// <expression> <op> <expression> <delimiter> <expression>
typedef struct TernaryOpNode
{
    struct ExprNode *c; // condition
    struct ExprNode *a; // expression a
    struct ExprNode *b; // expression b
} TernaryOpNode;
// <id> <(><expression><)>
typedef struct FunCallNode
{
    char n[nameLen];              // name
    struct ExprNode *pl[listLen]; // parameter list
} FunCallNode;

enum StateType
{
    IF = 1024,
    WHILE,
    BLOCK,
    RETURN,
    CONTINUE,
    BREAK, // StateNode.n = NULL
    EMPTY, // StateNode.n = NULL
    EXPR,
};
// switch node
typedef struct StateNode
{
    int t;   // type
    void *n; // node
    int l;   // line
} StateNode;
// specific statement node
typedef struct IfNode
{
    struct ExprNode *c; // condition
    struct StateNode a;
    struct StateNode b;
} IfNode;

typedef struct WhileNode
{
    struct ExprNode *c;
    struct StateNode s;
} WhileNode;

typedef struct BlockNode
{
    struct StateNode sl[listLen];
} BlockNode;

typedef struct Node
{
    int t;          // type
    void *n;        // node above
    struct Node *s; // successor
    int l;          // line
} Node;

#endif