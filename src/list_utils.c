#include <list.h>
#include "list_utils.h"

list_t *list_copy(list_t *self) {
  list_t *copy = list_new();

  list_node_t *node;
  list_iterator_t *it = list_iterator_new(self, LIST_HEAD);

  while ((node = list_iterator_next(it))) {
    list_rpush(copy, list_node_new(node->val));
  }

  list_iterator_destroy(it);

  return copy;
}

void list_toarray(list_t *self, void **result) {
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(self, LIST_HEAD);

  int i = 0;
  while ((node = list_iterator_next(it))) {
    result[i++] = node->val;
  }

  list_iterator_destroy(it);
}
