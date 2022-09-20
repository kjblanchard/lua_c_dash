#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int_queue* CreateIntQueue(int max)
{
    int_queue* queue = malloc(sizeof(*queue));
    queue->queue = calloc(max, sizeof(int));
    queue->max = max;
    queue->size = 0;

    return queue;
}

void DestroyIntQueue(int_queue* queue)
{
    free(queue->queue);
    queue->queue = NULL;
    free(queue);
    queue = NULL;
}

int PushIntQueue(int_queue* queue, int value)
{

    if(queue->size < queue->max)
    {
        queue->queue[queue->size] = value;
        ++queue->size;
        return 1;
    }
    puts("Fail");
    return 0;
}

int PopIntQueue(int_queue* queue)
{
    if(queue->size > 0)
    {
        int val = queue->queue[queue->size -1];
        --queue->size;
        return val;

    }
    return 0;
}
