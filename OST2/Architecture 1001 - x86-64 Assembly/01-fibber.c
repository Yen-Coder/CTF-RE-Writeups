#include <stdio.h>

unsigned int fibonacci(unsigned int n)
{
    if (n <= 1)
    {
        return n;
    }
    else
    {
        return (fibonacci(n - 1) + fibonacci(n - 2));
    }
}

int main()
{
    unsigned int n = 10;

    printf("First %d elements of the Fibonacci sequence: ", n);

    for (unsigned int i = 0; i < n; i++)
    {
        printf("%d ", fibonacci(i));
    }
    return 0;
}