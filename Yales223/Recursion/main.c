#include <stdio.h>

void printRangeRec(int start, int stop)
{
    if (start < stop)
    {
        printf("%d\n", start);
        printRangeRec(start + 1, stop);
    }
}

void printRangeRecursiveReversed(int start, int stop)
{
    if (start < stop)
    {
        printRangeRecursiveReversed(start + 1, stop);
        printf("%d\n", start);
    }
}

void printRangeMidSplit(int start, int stop)
{

    if (start < stop)
    {
        int mid = (start + stop) / 2;
        printRangeMidSplit(start, mid);
        printf("%d\n", mid);
        printRangeMidSplit(start + 1, stop);
    }
}

int main()
{

    return 0;
}