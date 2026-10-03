#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_INITIAL (16)
#define SIZE_MULTIPLIER (2)
struct array
{
    size_t size;
    int *data;
};

struct array *createArray(void)
{
    struct array *arr = malloc(sizeof(struct array));
    arr->size = SIZE_INITIAL;
    arr->data = calloc(arr->size, sizeof(int));
    return arr;
}

void arrayExpand(struct array *arr, size_t position)
{
    if (arr->size <= position)
    {
        size_t bigger = arr->size * SIZE_MULTIPLIER;
        if (position >= bigger)
        {
            bigger = position + 1;
        }
        arr->data = realloc(arr->data, sizeof(int) * bigger);
        memset(&arr->data[arr->size], 0, sizeof(int) * (bigger - arr->size));
        arr->size = bigger;
    }
}

void arrayprint(struct array *arr)
{
    int *i;
    // printf("\n------ARRAY------\n");
    printf("[");
    for (i = arr->data; i < &arr->data[arr->size]; i++)
    {
        if (i == &(arr->data[arr->size - 1]))
        {
            printf("%d", *i);
        }
        else
        {
            printf("%d, ", *i);
        }
    }
    printf("]\n");
}

int arrayGet(struct array *arr, size_t pos)
{
    return (arr->data[pos]);
}

int arraySet(struct array *arr, size_t pos, int val)
{
    arrayExpand(arr, pos);
    return arr->data[pos] = val;
}

void destroyArray(struct array *arr)
{
    free(arr->data);
    free(arr);
}

int main()
{
    struct array *arr = createArray();
    arrayprint(arr);
    arrayExpand(arr, 32);
    arrayprint(arr);
    printf("%d\n", arrayGet(arr, 3));
    for (int i = 0; i < arr->size; i++)
    {
        arraySet(arr, i, 2 * i);
    }
    arrayprint(arr);
    // destroyArray(arr);
    // arrayprint(arr);
    return 0;
}