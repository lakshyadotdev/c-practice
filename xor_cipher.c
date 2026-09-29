#include <stdio.h>

int length(char *s)
{
    char *p = s;
    while (*p != '\0')
    {
        p++;
    }
    return p - s;
}

void xorcipher(char *s)
{
    char key = 'K';
    int len = length(s);
    for (int i = 0; i < len; i++)
    {
        s[i] = *(s + i) ^ key;
    }
}

int main()
{
    char sample_string[] = "HarsH";
    printf("Original: %s\n", sample_string);
    xorcipher(sample_string);
    printf("Encrypting: %s\n", sample_string);
    xorcipher(sample_string);
    printf("Decrypting: %s\n", sample_string);
    return 0;
}