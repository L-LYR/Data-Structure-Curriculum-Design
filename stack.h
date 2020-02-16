#ifndef STACK_H_
#define STACK_H_
#include "error.h"
// stack basic size when initializing
#define initSize 64
// concat name
#define CONCAT(prefix, suffix) prefix##suffix
// stack type name
#define ELEM_TYPE(name) CONCAT(name, ElemType)
// function name
#define BASIC_PUSH(name) CONCAT(name, BasicPush)
#define PUSH(name) CONCAT(name, Push)
#define BASIC_POP(name) CONCAT(name, BasicPop)
#define POP(name) CONCAT(name, Pop)
#define INIT(name) CONCAT(name, Init)
#define DESTROY(name) CONCAT(name, Destroy)
#define GET_TOP(name) CONCAT(name, GetTop)
// stack template
#define STACK_TEMPLATE(type, name)                                     \
    typedef type ELEM_TYPE(name);                                      \
    static const size_t defaultSize = sizeof(ELEM_TYPE(name));         \
    typedef struct                                                     \
    {                                                                  \
        ELEM_TYPE(name) * e;                                           \
        size_t size, top;                                              \
    } name;                                                            \
    static void *BASIC_PUSH(name)(name * s, size_t size)               \
    {                                                                  \
        if (s->top + size >= s->size)                                  \
        {                                                              \
            if (s->size == 0)                                          \
                s->size = initSize;                                    \
            while (s->top + size >= s->size)                           \
                s->size += s->size >> 1;                               \
            s->e = realloc(s->e, s->size);                             \
            if (s->e == NULL)                                          \
            {                                                          \
                printf("Allocation of memory for ASR node failed!\n"); \
                exit(-1);                                              \
            }                                                          \
        }                                                              \
        void *ret = s->e + s->top;                                     \
        s->top += size;                                                \
        return ret;                                                    \
    }                                                                  \
    static void PUSH(name)(name * s, ELEM_TYPE(name) e)                \
    {                                                                  \
        *(ELEM_TYPE(name) *)BASIC_PUSH(name)(s, defaultSize) = e;      \
    }                                                                  \
    static void *BASIC_POP(name)(name * s, size_t size)                \
    {                                                                  \
        return s->e + (s->top -= size);                                \
    }                                                                  \
    static ELEM_TYPE(name) POP(name)(name * s)                         \
    {                                                                  \
        return *(ELEM_TYPE(name) *)BASIC_POP(name)(s, defaultSize);    \
    }                                                                  \
    static name *INIT(name)()                                          \
    {                                                                  \
        name *s = (name *)malloc(sizeof(name));                        \
        if (s == NULL)                                                 \
        {                                                              \
            printf("Allocation of memory for ASR node failed!\n");     \
            exit(-1);                                                  \
        }                                                              \
        s->e = NULL;                                                   \
        s->size = s->top = 0;                                          \
        return s;                                                      \
    }                                                                  \
    static void DESTROY(name)(name * s)                                \
    {                                                                  \
        free(s->e);                                                    \
        free(s);                                                       \
    }                                                                  \
    static ELEM_TYPE(name) GET_TOP(name)(name * s)                     \
    {                                                                  \
        return *(s->e + s->top - defaultSize);                         \
    }

#endif