#ifndef POINT_H
#define POINT_H

/**
 * @brief A point is a simple data container holding two ints, an x value and a y value.
 */
typedef struct Point 
{
    int x;
    int y;

} Point;

/**
 * @brief Creates a point
 *
 * @param x X variable
 * @param y Y variable
 *
 * @return a new point with the x and y
 */
inline Point CreatePoint(int x, int y)
{
    Point point;
    point.x = x;
    point.y = y;
    return point;
}

/**
 * @brief Creates a point with x and y as the value passed in
 *
 * @param xy the value for x and y
 *
 * @return A new point
 */
inline Point CreatePointXY(int xy)
{
    return CreatePoint(xy, xy);
}

/**
 * @brief Creates a point with 0 for x and y
 *
 * @return A zero point
 */
inline Point ZeroPoint()
{
    Point point = CreatePoint(0,0);
    return CreatePoint(0, 0);
}

/**
 * @brief Adds two points together
 *
 * @param lhs First operand
 * @param rhs Second operand
 *
 * @return A new point as the sum of two points
 */
inline Point AddPoint(Point lhs, Point rhs)
{
    Point point;
    point.x = lhs.x + rhs.x;
    point.y = lhs.y + rhs.y;
    return point;
}

#endif

