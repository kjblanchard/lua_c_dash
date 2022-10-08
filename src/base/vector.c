#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

#define VECTOR_INCREASE_MULTIPLIER 2

static int IncreaseVectorSize(vector* vector);

vector* CreateVector()
{
    const int initial_size = 2;
    vector* vector = malloc(sizeof(*vector));
    vector->data = calloc(initial_size, sizeof(int));
    vector->capacity = initial_size;
    vector->size = 0;

    return vector;
}

void DestroyVector(vector* vector)
{
    free(vector->data);
    vector->data = NULL;
    free(vector);
    vector = NULL;
}

void VectorPushBack(vector* vector, int value)
{
    if(vector->size == vector->capacity)
    {
        IncreaseVectorSize(vector);
    }
    vector->data[vector->size++] = value;
}

static int IncreaseVectorSize(vector* vector)
{
        vector->data = realloc(vector->data, sizeof(int) * vector->capacity * VECTOR_INCREASE_MULTIPLIER);
        vector->capacity *= VECTOR_INCREASE_MULTIPLIER;
        return 1;
}

unsigned char VectorRemoveItem(vector* vector, int value)
{
    if(!vector->size)
        return 0;
    unsigned char found = 0;
    for (int i = 0; i < vector->size; ++i) 
    {
        if(found)
        {
            vector->data[i -1] = vector->data[i];
        }
        else
        {
            if(vector->data[i] == value)
                found = 1;
        }
    }
    if(found)
    {
        vector->size = vector->size -1;
    }
    return found;

}
