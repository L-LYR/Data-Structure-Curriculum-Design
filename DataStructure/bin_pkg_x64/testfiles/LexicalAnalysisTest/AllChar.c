#include <stdio.h>
// this is a comment
enum constVal
{
    val1,
    val2,
};

int _666_, _777_, ____;
float f, ff, fff;
char c;
void fun(void);
void *ptr;
int ***ip;

int main(void)
{
    int i;

    i = -1000;
    _666_ = +0x666;
    _777_ = 0777;
    f = 1e9 + 7;
    ff = -12345.67890;
    fff = 1e-306;
    c = 'B';

    int a, b, c, d;

    c = (a == b) ? c : d;
    if (a == b && !(c <= d || d >= c))
        c = a << b;
    else if (a != b && a < b)
        d = a >> b;
    else if (c == d || c > b)
    {
        d = ++c;
        c = --d;
        b = a++;
        a = b--;
    }

    a = (4 * 7) / 2 + (1 - 9) % 4;

    char *p;
    p = (char *)ptr;

    *p = a | b ^ c & ~d;
    p[0] = '\0';
    p[1] = sizeof(char);
    p = "this is a constant string!";
    return 0;
}
