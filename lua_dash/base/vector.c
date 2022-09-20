#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

int_vector* CreateIntVector()
{
    const int initial_size = 2;
    int_vector* vector = malloc(sizeof(*vector));
    vector->vector = calloc(initial_size, sizeof(int));
    vector->max = initial_size;
    vector->size = 0;

    return vector;
}

void DestroyIntVector(int_vector* vector)
{
    free(vector->vector);
    vector->vector = NULL;
    free(vector);
    vector = NULL;
}

void AppendVectorInt(int_vector* vector, int value)
{
    if(vector->size == vector->max)
    {
        vector->size *= 2;
        vector->vector = reallocf(vector->vector,sizeof(int) * vector->size);
    }
    vector->vector[vector->size++] = value;
}

unsigned char RemoveVectorInt(int_vector* vector, int value)
{
    unsigned char found = 0;
    for (int i = 0; i < vector->size; ++i) 
    {
        if(found)
            vector->vector[i -1] = vector->vector[i];
        else
            if(vector->vector[i] == value)
                found = 1;
    }
    if(found)
        --vector->size;
    return found;

}
