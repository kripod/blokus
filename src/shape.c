#include <list.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "list_utils.h"
#include "point.h"
#include "shape.h"

void Shape_Destroy(Shape *self) {
  list_destroy(self->cells);
  free(self);
}

void Shape_Mirror(Shape *self, bool x, bool y) {
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(self->cells, LIST_HEAD);

  while ((node = list_iterator_next(it))) {
    Point *cell = node->val;

    // Mirror to the given axes
    cell->x = y ? self->width - 1 - cell->x : cell->x;
    cell->y = x ? self->height - 1 - cell->y : cell->y;
  }

  list_iterator_destroy(it);
}

void Shape_RotateRight(Shape *self) {
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(self->cells, LIST_HEAD);

  while ((node = list_iterator_next(it))) {
    Point *cell = node->val;

    // Rotate right by 90 degrees: (x, y) -> (y, -x)
    int oldX = cell->x;
    cell->x = self->height - 1 - cell->y;
    cell->y = oldX;
  }

  list_iterator_destroy(it);

  int oldWidth = self->width;
  self->width = self->height;
  self->height = oldWidth;
}

Shape *File_ReadNextShape(FILE *stream) {
  list_t *cells = list_new();
  cells->free = free;

  int x = 0;
  int y = 0;
  int maxX = x;

  // Read until the end of the current section
  for (char prev = '\0', curr = fgetc(stream);
       curr != EOF && (prev != '\n' || curr != '\n');
       prev = curr, curr = fgetc(stream)) {
    if (curr == 'X') {
      // Store cell at the current position
      Point *cell = malloc(sizeof(Point));
      cell->x = x;
      cell->y = y;

      list_rpush(cells, list_node_new(cell));
      maxX = x > maxX ? x : maxX;

    } else if (curr == '\n') {
      // Navigate to the next row
      x = 0;
      y += 1;
      continue;
    }

    // Navigate to the next column
    x += 1;
  }

  // Check whether a shape could be loaded successfully
  if (cells->len > 0) {
    Shape *shape = malloc(sizeof(Shape));
    shape->width = maxX + 1;
    shape->height = y;
    shape->cells = cells;
    return shape;
  } else {
    return NULL;
  }
}

list_t *File_ReadShapes(FILE *stream) {
  list_t *shapes = list_new();
  shapes->free = (void (*)(void *))(&Shape_Destroy);

  for (Shape *shape = File_ReadNextShape(stream); shape;
       shape = File_ReadNextShape(stream)) {
    list_rpush(shapes, list_node_new(shape));
  }

  return shapes;
}
