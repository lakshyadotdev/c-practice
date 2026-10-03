#include <stdio.h>
#include <stdlib.h>

#define STACK_EMPTY (0)
struct elt
{
    struct elt *next;
    int value;
};

typedef struct elt *Stack;

int stackEmpty(const Stack *s)
{
    return (*s == 0);
}

int stackPop(Stack *s)
{
    int ret;
    struct elt *e;
    if (!stackEmpty(s))
    {
        ret = (*s)->value;
        e = *s;
        *s = e->next;
        free(e);
        return ret;
    }
}

void stackPush(Stack *stack, int value)
{
    struct elt *elt;
    elt = malloc(sizeof(struct elt));
    elt->value = value;
    elt->next = *stack;
    *stack = elt;
}

void stackPrint(const Stack *s)
{
    struct elt *e;
    for (e = *s; e != 0; e = e->next)
    {
        printf("%d ", e->value);
    }
}

int main()
{
    int i;
    Stack s;
    s = STACK_EMPTY;

    for (i = 0; i < 5; i++)
    {
        printf("push %d\n", i);
        stackPush(&s, i);
    }
    stackPrint(&s);

    while (!stackEmpty(&s))
    {
        printf("pop gets %d\n", stackPop(&s));
    }
    stackPrint(&s);

    return 0;
}