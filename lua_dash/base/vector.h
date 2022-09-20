#pragma once

typedef struct vector {
    int* data;
    int capacity;
    int size;
} vector;

vector* CreateVector();
void DestroyVector(vector* vector);
void VectorPushBack(vector* vector, int value);
unsigned char VectorRemoveItem(vector* vector, int value);
