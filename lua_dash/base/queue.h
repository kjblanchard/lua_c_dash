#pragma once

/**
 * @brief A queue for value types that you can enque or dequeue.
 */
typedef struct queue
{
    int* data;
    int capacity;
    int rear;
    int front;
} queue;
/**
 * @brief Creates a queue of value types that does not grow in size.
 *
 * @param The capacity of this queue.
 *
 * @return A newly created queue.
 */
queue* CreateQueue(int capacity);
/**
 * @brief Destroys a queue and releases it's data.
 *
 * @param queue The queue to destroy.
 */
void DestroyQueue(queue* queue);
/**
 * @brief Adds a value to the rear of the queue
 *
 * @param queue The queue to enqueue to
 * @param value The value to enqueue to
 *
 * @return 1 if successfully added, or 0 if the queue is full.
 */
int Enqueue(queue* queue, int value);
/**
 * @brief Pops the front of the queue (returns it to you.)
 *
 * @param queue The queue to pop
 *
 * @return The value at the front of the queue, or 0 if the queue is empty.
 */
int Dequeue(queue* queue);
/**
 * @brief Checks to see if the queue is full.
 *
 * @param queue The queue to check
 *
 * @return 1 if it is full, 0 if not.
 */
int QueueIsFull(queue* queue);
/**
 * @brief Gets the amount of space left in the queue.
 *
 * @param queue The queue to query.
 *
 * @return The amount of space left in the queue.
 */
int QueueRemaining(queue* queue);
/**
 * @brief Checks to see if the queue is empty.
 *
 * @param queue The queue to check.
 *
 * @return 1 if the queue is empty, 0 if not.
 */
int QueueIsEmpty(queue* queue);
