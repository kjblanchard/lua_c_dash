/**
 * @file vector2.h
 * @brief A object that holds 2 floats.
 * @author Kevin Blanchard
 * @version 0.0.1
 * @date 2022-10-07
 */
#pragma once

/**
 * @brief A Vector2 is a simple data structure that holds 3 floats.
 */
typedef struct Vector2
{
    float x;
    float y;

} Vector2;
/**
 * @brief Creates a Vector2
 *
 * @param x X variable
 * @param y Y variable
 *
 * @return a new vector2 with the x and y
 */
Vector2 CreateVector2(float x, float y);
/**
 * @brief Creates a Vector2 with x and y as the value passed in
 *
 * @param xy the value for x and y
 *
 * @return A new vector2
 */
Vector2 CreateVector2XY(float xy);
/**
 * @brief Creates a Vector2  with 0 for x and y
 *
 * @return A zero vector2
 */
Vector2 ZeroVector2();
/**
 * @brief Adds two vector2s together
 *
 * @param lhs First operand
 * @param rhs Second operand
 *
 * @return A new vector2 as the sum of two points
 */
Vector2 AddVector2(Vector2 lhs, Vector2 rhs);
