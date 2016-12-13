#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

/** @file */

#include <list.h>

/**
 * Copies each element of a list to a new list by node value.
 * @param self Instance to apply the function on.
 */
list_t *list_copy(list_t *self);

/**
 * Converts a list to an array.
 * @param self Instance to apply the function on.
 * @param result Destination of the result.
 */
void list_toarray(list_t *self, void **result);

#endif  // LIST_H_INCLUDED
