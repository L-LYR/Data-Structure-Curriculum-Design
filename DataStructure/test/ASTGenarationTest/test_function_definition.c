void print(char *a);
// normal function
int main(void)
{
    print("Hello World!\n");
    return 0;
}
// recursive function
int fibonacci(int n)
{
    if (n <= 0)
        return 0;
    else if (n == 1 || n == 2)
        return 1;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

void a(void);
void b(void) { a(); }
void a(void) { b(); }