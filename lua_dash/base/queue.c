#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "../core/debug.h"

/**
 * @brief Moves all the data in the queue to the front, so that we can add more data
 *
 * @param queue The queue to move data with.
 */
static void MoveDataToFrontOfQueue(queue* queue);

queue* CreateQueue(int capacity)
{
    queue* queue = malloc(sizeof(*queue));
    queue->data = calloc(capacity, sizeof(int));
    queue->capacity = capacity;
    queue->rear = queue->front = 0;
    return queue;
}

void DestroyQueue(queue* queue)
{
    free(queue->data);
    queue->data = NULL;
    free(queue);
    queue = NULL;
}

int Enqueue(queue* queue, int value)
{
    //If the rear is at the capacity
    if(queue->rear == queue->capacity)
    {
        //And if the queue is in the front, we are full.
        if(queue->front == 0)
        {
            LogWarn("Trying to add to a full queue.");
            return 0;
        }
        //If we are at the capacity, but the front is not at the beginning, move all data to the front.
        else
        {
            MoveDataToFrontOfQueue(queue);
        }
    }
    queue->data[queue->rear++] = value;
    return 1;
}

static void MoveDataToFrontOfQueue(queue* queue)
{
    int insert_pos = 0;
    int new_rear = queue->rear - queue->front;
    for(size_t i = queue->front; i < queue->rear; ++i)
    {
        queue->data[insert_pos++] = queue->data[i];

    }

    queue->front = 0;
    queue->rear = new_rear;

}

int Dequeue(queue* queue)
{
    //If rear is at position 0, we are empty.
    if(!queue->rear)
    {
        LogWarn("Trying to pop an empty queue");
        return 0;
    }
    int return_data = queue->data[queue->front];
    //If the front and the rear are equal after moving the front up, then there is nothing in the queue so lets move to the front.
    if(++queue->front == queue->rear)
        queue->front = queue->rear = 0;
    return return_data;
}
int QueueIsFull(queue* queue)
{
    return queue->rear == queue->capacity && queue->front == 0;
}
int QueueRemaining(queue* queue)
{
    return queue->front + (queue->capacity - queue->rear);
}
int QueueIsEmpty(queue* queue)
{
    return queue->front == queue->rear;
}
