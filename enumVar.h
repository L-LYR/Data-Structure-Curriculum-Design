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
    Len, // length of Name, easy for display
    IdSize
};

// types of variable/function
enum type
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

enum NodeType
{
    EnumDec = 512,
    VarDec,
    FunDec,

    UnaryExpr,
    BinaryExpr,
    FunCall,
    CondExpr,
    UnitExpr, // only for single Variable, String, Num, Empty.

    // TODO: statements
    FunDef,
};
// fields of AST node
enum node
{
    nodeType,
    name,
    basetype = 1,
    range,
    list,
    nodeSize,
};