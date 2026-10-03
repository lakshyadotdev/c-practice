#include <stdio.h>
#include <stdlib.h>

struct elt
{
    struct elt *next;
    int value;
};

struct queue
{
    struct elt *head;
    struct elt *tail;
};

struct queue *queueCreate(void)
{
    struct queue *q;
    q = malloc(sizeof(struct queue));
    q->head = q->tail = 0;
    return q;
}

void enq(struct queue *q, int value)
{
    struct elt *e;
    e = malloc(sizeof(struct elt));
    e->value = value;
    e->next = 0;
    if (q->head == 0)
    {
        q->head = e;
    }
    else
    {
        q->tail->next = e;
    }
    q->tail = e;
}

int queueEmpty(const struct queue *q)
{
    return (q->head == 0);
}

int deq(struct queue *q)
{
    int ret;
    struct elt *e;
    if (!queueEmpty(q))
    {
        ret = q->head->value;
        e = q->head;
        q->head = e->next;
        free(e);
        return ret;
    }
}
void queuePrint(struct queue *q)
{
    struct elt *e;
    for (e = q->head; e != NULL; e = e->next)
    {
        printf("%d ", e->value);
    }
    printf("\n");
}

void queueDestroy(struct queue *q)
{
    while (!queueEmpty(q))
    {
        deq(q);
    }
    free(q);
}

int main(int argc, char const *argv[])
{
    int i;
    struct queue *q;
    q = queueCreate();
    for (i = 0; i < 5; i++)
    {
        printf("enq %d\n", i);
        enq(q, i);
        queuePrint(q);
    }

    while (!queueEmpty(q))
    {
        printf("deq gets %d\n", deq(q));
        queuePrint(q);
    }

    queueDestroy(q);
    return 0;
}
