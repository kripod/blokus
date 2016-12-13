#include "player.h"
#include "shape.h"

Player Player_Create(Color color, int default_score, list_t *placeable_shapes) {
  return (Player){.color = color,
                  .score = default_score,
                  .placeable_shapes = placeable_shapes,
                  .selected_shape_node = placeable_shapes->head};
}

void Player_Destroy(Player *self) { list_destroy(self->placeable_shapes); }

int Player_ComparePointers(const void *pa, const void *pb) {
  Player *a = *(Player * const *)pa;
  Player *b = *(Player * const *)pb;
  return b->score - a->score;
}

Shape *Player_GetSelectedShape(Player *self) {
  return self->selected_shape_node->val;
}

Shape *Player_NextShape(Player *self) {
  return (self->selected_shape_node = self->selected_shape_node->next
                                          ? self->selected_shape_node->next
                                          : self->placeable_shapes->head)
      ->val;
}

Shape *Player_PrevShape(Player *self) {
  return (self->selected_shape_node = self->selected_shape_node->prev
                                          ? self->selected_shape_node->prev
                                          : self->placeable_shapes->tail)
      ->val;
}
