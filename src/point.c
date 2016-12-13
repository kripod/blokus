#include <stdbool.h>
#include "point.h"

bool Point_Equals(Point *a, Point *b) { return a->x == b->x && a->y == b->y; }

Point Point_Add(Point *a, Point *b) {
  return (Point){a->x + b->x, a->y + b->y};
}

Point Point_Min(Point *a, Point *b) {
  return (Point){a->x < b->x ? a->x : b->x, a->y < b->y ? a->y : b->y};
}
