#include <stdio.h>

int strlen(char *s)
{
    char *p = s;
    while (*p != '\0')
    {
        p++;
    }
    return p - s;
}

int main()
{
    printf("%d\n", strlen("HELLLO"));
    return 0;
}