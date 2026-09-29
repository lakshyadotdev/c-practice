#include <stdio.h>

char *encLookupTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void printb(char *buffer, size_t t, int base)
{
    int count = 0;
    for (size_t i = 0; i < t; i++)
    {
        size_t num = buffer[i];
        for (int bit = 7; bit >= 0; bit--)
        {
            if (count == base)
            {
                printf(" | ");
                count = 0;
            }
            printf("%zu", (num >> bit) & 1);
            count++;
        }
    }
    printf("\n");
}

void encode(char *buffer, size_t t)
{
    int bit6 = ((t * 8) / 6);
    int n_padding = 3 - bit6 % 4;
    char encodedStr[bit6 + n_padding + 1];
    int count = 0;
    printf("%d %d \n", bit6 + n_padding + 1);
    char temp[6] = {0};
    printf("No. of proper 6 Bits: %d\nNo. of '=' needed: %d \n", bit6, n_padding);
    for (int i = 0; i < t; i++)
    {
        size_t num = buffer[i];
        for (int bit = 7; bit >= 0; bit--)
        {
            if (count == 6)
            {
                count = 0;
            }

            printf("%zu", (num >> bit) & 1);
            temp[count] = (num >> bit) & 1;
            count++;
        }
    }
}

int main()
{
    char sample_string[] = "LJK";
    printb(sample_string, sizeof(sample_string) / sizeof(sample_string[0]), 6);
    encode(sample_string, sizeof(sample_string) / sizeof(sample_string[0]));
    return 0;
}