// TODO: need to be supported: double float long long short array

enum word
{
    ID = 1,
    KEY,
    CONST,
    OP,
    DELIM,
};

// tokens and classes
// operators are in precedence order
enum class_token
{
    // class
    Num = 256,
    Fun,
    Glo,
    Loc,
    Id,
    // token
    Char,
    Else,
    Enum,
    If,
    Int,
    Return,
    Sizeof,
    While,
    Break,
    Continue,
    Void,
    Assign,
    Cond,
    Lor,
    Lan,
    Or,
    Xor,
    And,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Shl,
    Shr,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Inc,
    Dec,
    Brak,

};

// fields of identifier
enum identifier
{
    Token, // placeholder
    Hash,  // name hash
    Name,  // pointer to name
    Type,  // variable type, INT CHAR or +PTR
    Class, // constant class Num Glo Loc Fun
    Value, // value of variable
    BType, // B_____ is for global variable
    BClass,
    BValue,
    Len,    // length of Name, easy for display
    status, // whether function has been defined
    IdSize
};

// types of variable/function
enum var_fun_type
{
    CHAR = 1,
    INT,
    BASE = 3,
    PTR = 2
};

// type of declaration.
enum dec
{
    Global,
    Local
};

enum ast_node_type
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

// fields of AST node
enum ast_node
{
    nodetype,
    nodename,
    nodelist,
    base,
    body = 3,
    nodesize,
};

enum glo_dec_list
{
    varName,
    ptr,
    paralist,
    GloDecNodeSize,
};

enum para_list
{
    paraName,
    paraType,
    ParaNodeSize,
};