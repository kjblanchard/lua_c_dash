/**
 * @file stack.h
 * @brief A stack is a fixed size data structure that does LIFO with push and pop
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-16
 */

#pragma once

typedef struct Stack
{
    int* data;
    int capacity;
    int size;

} Stack;

Stack* CreateStack(int capacity);
int PopStack(Stack* stack);
void PushStack(Stack* stack, int item);
void DestroyStack(Stack* stack);
