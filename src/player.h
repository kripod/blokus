#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <list.h>
#include "color.h"
#include "shape.h"

/**
 * Represents a player of a Game.
 */
typedef struct Player {
  /**
   * Color of the player.
   */
  Color color;

  /**
   * List of the player's unplaced Shape structures.
   */
  list_t *placeable_shapes;

  /**
   * List node of the player's currently selected Shape.
   */
  list_node_t *selected_shape_node;

  /**
   * Score of the player (based on the amount of cells occupied on the Board).
   */
  int score;
} Player;

Player Player_Create(Color color, int default_score, list_t *placeable_shapes);
void Player_Destroy(Player *self);

int Player_ComparePointers(const void *pa, const void *pb);

Shape *Player_GetSelectedShape(Player *self);
Shape *Player_NextShape(Player *self);
Shape *Player_PrevShape(Player *self);

#endif  // PLAYER_H_INCLUDED
