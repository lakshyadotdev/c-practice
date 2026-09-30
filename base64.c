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
    int total_bits = t * 8;
    int complete_6bit = total_bits / 6;
    int last_bit_remainder = total_bits % 6;
    int groups = complete_6bit + (last_bit_remainder != 0);
    int padding_bytes = (4 - (groups % 4)) % 4;
    printf("-------------------\nTotalBytes: %zu * 8 => %d \nComplete 6Bits: %d\nRemainderBits: %d\nGroups: %d\nPadding Bytes required: %d\n", t, total_bits, complete_6bit, last_bit_remainder, groups, padding_bytes);
    for (int b = 0; b < t; b++)
    {
        int byte = buffer[b];
        int temp[6];
        for (int k = 7; k >= 0; k--)
        {
            int bit = byte >> k & 1;
                }
    }
}

int main()
{
    char sample_string[] = "aaaaa";
    printb(sample_string, sizeof(sample_string) / sizeof(sample_string[0]) - 1, 6);
    encode(sample_string, sizeof(sample_string) / sizeof(sample_string[0]) - 1);
    return 0;
}