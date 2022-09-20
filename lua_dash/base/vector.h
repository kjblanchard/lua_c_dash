#pragma once

typedef struct int_vector {
    int* vector;
    int max;
    int size;
} int_vector;

int_vector* CreateIntVector();
void DestroyIntVector(int_vector* vector);
//TODO handle failures.
void AppendVectorInt(int_vector* vector, int value);
unsigned char RemoveVectorInt(int_vector* vector, int value);
