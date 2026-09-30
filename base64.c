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
    int padding_bytes = (complete_6bit + (last_bit_remainder != 0)) % 3;
    printf("-------------------\nTotalBytes: %zu => %d \nComplete 6Bits: %d\nRemainderBits: %d\nPadding Bytes required: %d\n", t, total_bits, complete_6bit, last_bit_remainder, padding_bytes);
}

int main()
{
    char sample_string[] = "a";
    printb(sample_string, sizeof(sample_string) / sizeof(sample_string[0]), 6);
    encode(sample_string, sizeof(sample_string) / sizeof(sample_string[0]));
    return 0;
}