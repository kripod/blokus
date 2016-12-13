#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <list.h>
#include <stdbool.h>
#include "player.h"
#include "shape.h"

/**
 * Keeps track of players in a game.
 */
typedef struct Game {
  /**
   * List of Player structs.
   */
  list_t *players;

  /**
   * List node of the Player in turn.
   */
  list_node_t *current_player_node;

  /**
   * Amount of consecutive passes between players.
   */
  int num_consecutive_passes;

  /**
   * Determines whether the game has ended.
   */
  bool has_ended;
} Game;

Game Game_Create(int num_players, list_t *shapes);
void Game_Destroy(Game *self);

Player *Game_GetCurrentPlayer(Game *self);
Player *Game_NextPlayer(Game *self, bool has_passed);

bool Game_HandleEvent(Game *self, SDL_Event *e);

#endif  // GAME_H_INCLUDED
