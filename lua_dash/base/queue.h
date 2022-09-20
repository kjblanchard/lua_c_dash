#pragma once

typedef struct int_queue {
    int* queue;
    int max;
    int size;
} int_queue;

int_queue* CreateIntQueue(int max);

void DestroyIntQueue(int_queue* queue);

int PushIntQueue(int_queue* queue, int value);

int PopIntQueue(int_queue* queue);
