#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "enumVar.h"

extern int line; // memoryPool.c

// TODO: user can change the size of memory pool
int *symbols,   // symbol table
    *currentId, // currentId in symbol table
    token,      // current token
    tokenVal;   // value of current token

char *src, *oldSrc, // pointer to the source code string
    *data,          // data segment used to save string
    *lastPos;       // help to mark the beginning of identifier or string
void getIdentifier()
{
    int hash;
    // parse identifier
    lastPos = src - 1;
    hash = token;
    // calculate identifier hash, helpful to compare
    while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
    {
        hash = hash * 147 + *src;
        src++;
    }

    // look for existing identifier, linear search
    currentId = symbols;
    while (currentId[Token])
    {
        if (currentId[Hash] == hash && !memcmp((char *)currentId[Name], lastPos, src - lastPos))
        {
            //found one, return
            token = currentId[Token];
            return;
        }
        currentId = currentId + IdSize;
    }

    // store new ID
    currentId[Name] = (int)lastPos;
    currentId[Hash] = hash;
    currentId[Len] = (src - lastPos);
    token = currentId[Token] = Id;
    return;
}

void getInt()
{
    // parse number, three kinds: dec(123) hex(0x123) oct(017)
    tokenVal = token - '0';
    if (tokenVal > 0) // dec, starts with [1-9]
    {
        token = *src;
        while (token >= '0' && token <= '9')
        {
            tokenVal = tokenVal * 10 + token - '0';
            token = *++src;
        }
    }
    else // starts with 0
    {
        if (*src == 'x' || *src == 'X')
        {
            // hex
            token = *++src;
            while ((token >= '0' && token <= '9') ||
                   (token >= 'a' && token <= 'f') ||
                   (token >= 'A' && token <= 'F'))
            {
                // a trick
                tokenVal = tokenVal * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
                token = *++src;
            }
        }
        else
        {
            // oct
            token = *src;
            while (token >= '0' && token <= '7')
            {
                tokenVal = tokenVal * 8 + token - '0';
                token = *++src;
            }
        }
    }

    token = Num;
    return;
}
// NOTICE:
//      1. This function not only parse a string but also parse a single charactor
//      2. If it is a single charactor, we will give it the Num class.
void getString()
{
    lastPos = data;
    while (*src != 0 && *src != token)
    {
        tokenVal = *src++;
        if (token == '"')
            *data++ = tokenVal;
    }
    ++data; // to leave a '\0' between two string
    ++src;
    // if it is a single character, return Num token
    if (token == '"')
        tokenVal = (int)lastPos;
    else
        token = Num;
    return;
}
// This function will get operators from src.
// I try my best to shorten it which still have about 100 lines,
// but I think it is not so hard to understand it
// because of its simple logic.
void getOperator()
{
    if (token == '=') // == and =
    {
        if (*src == '=')
        {
            src++;
            token = Eq;
        }
        else
            token = Assign;
    }
    else if (token == '!') // ! and !=
    {
        if (*src == '=')
        {
            src++;
            token = Ne;
        }
        // else
        //     token = Not;
    }
    else if (token == '<') // <= << and <
    {
        if (*src == '=')
        {
            src++;
            token = Le;
        }
        else if (*src == '<')
        {
            src++;
            token = Shl;
        }
        else
            token = Lt;
    }
    else if (token == '>') // >= >> and >
    {
        if (*src == '=')
        {
            src++;
            token = Ge;
        }
        else if (*src == '>')
        {
            src++;
            token = Shr;
        }
        else
            token = Gt;
    }
    else if (token == '|') // || and |
    {
        if (*src == '|')
        {
            src++;
            token = Lor;
        }
        else
            token = Or;
    }
    else if (token == '&') // && and &
    {
        if (*src == '&')
        {
            src++;
            token = Lan;
        }
        else
            token = And;
    }
    else if (token == '+') // special for + ++ +<num>
    {
        if (*src == '+')
        {
            src++;
            token = Inc;
        }
        else
            token = Add;
    }
    else if (token == '-') // special for - -- -<num>
    {
        if (*src == '-')
        {
            src++;
            token = Dec;
        }
        else
            token = Sub;
    }
    else if (token == '^')
        token = Xor;
    else if (token == '%')
        token = Mod;
    else if (token == '*') // Pointers will be parsed as Mul first.
        token = Mul;
    else if (token == '[')
        token = Brak;
    else if (token == '?')
        token = Cond;
    else if (token == ']' || token == '~')
        // directly return the character as token;
        return;
    else
        printf("Line %d: Unknow token %c\n", line, token);
}

int nextToken()
{
    while (token = *src) // loop until EOF
    {
        ++src;
        if (token == '\n' || token == ' ' || token == '\r' || token == '\t') // next line
        {
            if (token == '\n')
                ++line;
        }
        else if (token == '#') // macro, no suported
        {
            while (*src != 0 && *src != '\n')
                src++;
        }
        else if ((token >= 'a' && token <= 'z') ||
                 (token >= 'A' && token <= 'Z') ||
                 (token == '_')) // identifier
        {
            getIdentifier();
            return token == Id ? ID : KEY;
        }
        else if (token >= '0' && token <= '9') // integer
        {
            getInt();
            return CONST;
        }
        else if (token == '/') // comment
        {
            if (*src == '/')
            {
                // skip one-line comment
                while (*src != 0 && *src != '\n')
                    ++src;
            }
            else
            {
                // divide operator
                token = Div;
                return OP;
            }
        }
        else if (token == '"' || token == '\'') // string
        {
            getString();
            return CONST;
        }
        else if (token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ',' || token == ':')
        {
            return DELIM;
        }
        else // operator and delimiter
        {
            getOperator();
            return OP;
        }
    }
    return 0;
}
void match(int tk)
{
    if (token == tk)
        nextToken();
    else
    {
        if (tk > 256)
            printf("Line %d: expected token: %d\n", line, tk);
        else
            printf("Line %d: expected token: '%c'\n", line, tk);
        exit(-1);
    }
}