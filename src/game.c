#include <list.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "color.h"
#include "game.h"
#include "list_utils.h"
#include "player.h"
#include "shape.h"

Game Game_Create(int num_players, list_t *shapes) {
  list_t *players = list_new();
  players->free = (void (*)(void *))(&Player_Destroy);

  // Calculate default score of each player
  int default_score = 0;
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(shapes, LIST_TAIL);

  while ((node = list_iterator_next(it))) {
    Shape *shape = node->val;
    default_score -= shape->cells->len;
  }

  list_iterator_destroy(it);

  // Inizialize list of players
  for (int color = BLUE; color <= num_players; ++color) {
    Player *player = malloc(sizeof(Player));
    *player = Player_Create(color, default_score, list_copy(shapes));
    list_rpush(players, list_node_new(player));
  }

  return (Game){.players = players,
                .current_player_node = players->head,
                .num_consecutive_passes = 0,
                .has_ended = false};
}

void Game_Destroy(Game *self) { list_destroy(self->players); }

Player *Game_GetCurrentPlayer(Game *self) {
  return self->current_player_node->val;
}

Player *Game_NextPlayer(Game *self, bool has_passed) {
  Player *player =
      (self->current_player_node = self->current_player_node->next
                                       ? self->current_player_node->next
                                       : self->players->head)
          ->val;

  if (has_passed || player->placeable_shapes->len == 0) {
    if (++self->num_consecutive_passes == (int)self->players->len) {
      self->has_ended = true;
      return NULL;
    }
  } else {
    self->num_consecutive_passes = 0;
  }

  // Ensure that only a player with placeable shapes may come next
  return player->placeable_shapes->len > 0 ? player
                                           : Game_NextPlayer(self, has_passed);
}

bool Game_HandleEvent(Game *self, SDL_Event *e) {
  if (self->has_ended) return false;

  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_RIGHT:
      case SDLK_d: {
        Player *current_player = Game_GetCurrentPlayer(self);
        Player_NextShape(current_player);
        return true;
      }

      case SDLK_LEFT:
      case SDLK_a: {
        Player *current_player = Game_GetCurrentPlayer(self);
        Player_PrevShape(current_player);
        return true;
      }

      case SDLK_UP:
      case SDLK_w: {
        Player *current_player = Game_GetCurrentPlayer(self);
        Shape *selected_shape = Player_GetSelectedShape(current_player);
        Shape_Mirror(selected_shape, true, false);
        return true;
      }

      case SDLK_DOWN:
      case SDLK_s: {
        Player *current_player = Game_GetCurrentPlayer(self);
        Shape *selected_shape = Player_GetSelectedShape(current_player);
        Shape_Mirror(selected_shape, false, true);
        return true;
      }

      case SDLK_SPACE: {
        Player *current_player = Game_GetCurrentPlayer(self);
        Shape *selected_shape = Player_GetSelectedShape(current_player);
        Shape_RotateRight(selected_shape);
        return true;
      }

      case SDLK_p: {
        Game_NextPlayer(self, true);
        return true;
      }
    }
  }

  return false;
}
