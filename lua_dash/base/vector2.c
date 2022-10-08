#include "vector2.h"

Vector2 CreateVector2(float x, float y)
{
    Vector2 vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

Vector2 CreateVector2XY(float xy)
{
    return CreateVector2(xy, xy);
}

Vector2 ZeroVector2()
{
    return CreateVector2(0, 0);
}

Vector2 AddVector2(Vector2 lhs, Vector2 rhs)
{
    Vector2 vec2;
    vec2.x = lhs.x + rhs.x;
    vec2.y = lhs.y + rhs.y;
    return vec2;
}
