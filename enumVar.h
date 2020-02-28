#ifndef ENUMVAR_H
#define ENUMVAR_H

enum word
{
    ID = 1,
    KEY,
    CONST_INT,
    CONST_FLO,
    CONST_STR,
    OP,
    DELIM,
};

// tokens and classes
// operators are in precedence order
enum class_token
{
    // class
    Num = 256,
    Flo,
    Fun,
    Glo,
    Loc,
    Id,
    // token
    Void,
    Int,
    Float,
    Char,
    Else,
    Enum,
    If,
    Return,
    Sizeof,
    While,
    For,
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
    BType, // B____ is for global variable
    BClass,
    BValue,
    Len, // length of Name, easy for display
    IdSize
};

// types of variable/function
enum var_type
{
    VOID,
    INT,
    FLOAT,
    CHAR,
    PTR,
};

#endif