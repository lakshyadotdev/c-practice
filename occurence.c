#include <stdio.h>

struct token
{
    int ascii;
    char *val;
};

int length(char *s)
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
    char *myString = "This is my paragraph. This is my c program paragraph.";
    int l = length(myString);
    struct token tokens[l];
    char *i;
    int c = 0;
    for (i = myString; *i != '\0'; i++)
    {
        tokens[c].val = i;

        c++;
    }

    return 0;
}