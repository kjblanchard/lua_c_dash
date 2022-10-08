/**
 * @file vector.h
 * @brief An array that can dynamically grow as you add elements to it, as well as remove items.
 * @author Kevin Blanchard
 * @version 0.1
 * @date 2022-09-28
 */
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
