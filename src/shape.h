#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include <list.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * A rotatable shape which can be placed on a Board.
 */
typedef struct Shape {
  /**
   * Width of the shape (measured in cells).
   */
  int width;

  /**
   * Height of the shape (measured in cells).
   */
  int height;

  /**
   * List of cells contained by the shape.
   */
  list_t *cells;
} Shape;

void Shape_Destroy(Shape *shape);

void Shape_Mirror(Shape *self, bool x, bool y);
void Shape_RotateRight(Shape *self);

Shape *File_ReadNextShape(FILE *stream);
list_t *File_ReadShapes(FILE *stream);

#endif  // SHAPE_H_INCLUDED
