#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

/** @file */

#include <stdbool.h>

/**
 * 2D point structure.
 */
typedef struct Point {
  /**
   * X-coordinate of the point.
   */
  int x;

  /**
   * Y-coordinate of the point.
   */
  int y;
} Point;

/**
 * Determines whether two Point structures contain the same coordinates.
 */
bool Point_Equals(Point *a, Point *b);

/**
 * Adds two Point structures together.
 */
Point Point_Add(Point *a, Point *b);

/**
 * Calculates a Point composed by the littlest coordinates of each parameter.
 */
Point Point_Min(Point *a, Point *b);

#endif  // POINT_H_INCLUDED
