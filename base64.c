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
    char encodedArr[groups + padding_bytes];
    printf("-------------------\nTotalBytes: %zu * 8 => %d \nComplete 6Bits: %d\nRemainderBits: %d\nGroups: %d\nPadding Bytes required: %d\n", t, total_bits, complete_6bit, last_bit_remainder, groups, padding_bytes);
    int temp[6] = {0};
    int count = 0;
    int encodedCounter = 0;
    for (int b = 0; b < t; b++)
    {
        int byte = buffer[b];
        int sum = 0;
        for (int k = 7; k >= 0; k--)
        {
            if (count >= 6)
            {
                int n = 32;
                for (int j = 0; j < 6; j++)
                {
                    sum += n * temp[j];
                    n /= 2;
                }

                count = 0;
                encodedArr[encodedCounter] = encLookupTable[sum];
                // printf("%c", encLookupTable[sum]);
                encodedCounter++;
            }
            int bit = byte >> k & 1;
            temp[count] = bit;
            count++;
        }
    }
    // printf("\nCOUNT: %d\n", count);
    int n = 32;
    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += temp[i] * n;
        n /= 2;
    }
    for (int i = count; i < 6; i++)
    {
        n /= 2;
    }
    encodedArr[encodedCounter] = encLookupTable[sum];
    encodedCounter++;
    for (int i = 0; i < padding_bytes; i++)
    {
        encodedArr[encodedCounter] = '=';
        encodedCounter++;
    }
    encodedArr[encodedCounter] = '\0';
    printf("%s\n", encodedArr);
}

int main()
{
    char sample_string[] = "wow";
    // printb(sample_string, sizeof(sample_string) / sizeof(sample_string[0]) - 1, 6);
    encode(sample_string, sizeof(sample_string) / sizeof(sample_string[0]) - 1);
    return 0;
}