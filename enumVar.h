#ifndef ENUMVAR_H
#define ENUMVAR_H
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
enum var_type
{
    VOID,
    CHAR,
    INT,
    PTR,
};

// type of declaration.
enum dec
{
    Global,
    Local
};

#endif