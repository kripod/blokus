#include <list.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"
#include "color.h"
#include "config.h"
#include "player.h"
#include "point.h"
#include "shape.h"

Board Board_Create(Game *game, int width, int height, SDL_Renderer *renderer,
                   const SDL_Rect *display_rect) {
  Board board = (Board){.game = game,
                        .width = width,
                        .height = height,
                        .renderer = renderer,
                        .display_rect = display_rect};

  // Initialize grid of cells
  board.cells = (Color **)calloc(height, sizeof(Color *));
  board.cells[0] = (Color *)calloc(width * height, sizeof(Color));
  for (int y = 1; y < height; ++y) {
    board.cells[y] = board.cells[0] + y * height;
  }

  // Initialize board graphics if necessary
  if (renderer && display_rect) {
    board.is_frozen = false;

    board.textures = malloc(2 * sizeof(SDL_Texture *));
    for (int i = 0; i < kBoardNumLayers; ++i) {
      board.textures[i] = SDL_CreateTexture(
          renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
          display_rect->w - display_rect->w % width,
          display_rect->h - display_rect->h % height);
    }

    // Make the overlay transparent
    SDL_SetTextureBlendMode(board.textures[1], SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(board.textures[1], kBoardOverlayAlpha);

    board.overlay_cell_pos = (Point){-1, -1};
    board.overlay_shape = NULL;

    // Redraw every layer
    Board_RenderUpdateAll(&board);
  }

  return board;
}

void Board_Destroy(Board *self) {
  if (self->renderer) {
    for (int i = 0; i < kBoardNumLayers; ++i) {
      SDL_DestroyTexture(self->textures[i]);
    }

    free(self->textures);
  }

  free(self->cells[0]);
  free(self->cells);
}

bool Board_HasCell(Board *self, Point *pos) {
  return pos->x >= 0 && pos->y >= 0 && pos->x < self->width &&
         pos->y < self->height;
}

bool Board_IsCellOccupied(Board *board, Point *pos) {
  return board->cells[pos->x][pos->y] != NONE;
}

bool Board_IsCornerCell(Board *board, Point *pos) {
  return ((pos->x == 0 || pos->x == board->width - 1) &&
          (pos->y == 0 || pos->y == board->height - 1));
}

bool Board_CellHasValidCornerToCornerContact(Board *board, Point *pos,
                                             Color color) {
  if (pos->x > 0) {
    if (pos->y > 0 && board->cells[pos->x - 1][pos->y - 1] == color)
      return true;

    if (pos->y + 1 < board->height &&
        board->cells[pos->x - 1][pos->y + 1] == color)
      return true;
  }

  if (pos->x + 1 < board->width) {
    if (pos->y > 0 && board->cells[pos->x + 1][pos->y - 1] == color)
      return true;

    if (pos->y + 1 < board->height &&
        board->cells[pos->x + 1][pos->y + 1] == color)
      return true;
  }

  return false;
}

bool Board_CellHasInvalidEdgeToEdgeContact(Board *board, Point *pos,
                                           Color color) {
  return (pos->x > 0 && board->cells[pos->x - 1][pos->y] == color) ||
         (pos->y > 0 && board->cells[pos->x][pos->y - 1] == color) ||
         (pos->x + 1 < board->width &&
          board->cells[pos->x + 1][pos->y] == color) ||
         (pos->y + 1 < board->height &&
          board->cells[pos->x][pos->y + 1] == color);
}

bool Board_IsShapePlaceable(Board *board, Point *pos, Color color,
                            Shape *shape) {
  bool isInCorner = false;
  bool hasValidCornerToCornerContact = false;

  list_node_t *node;
  list_iterator_t *it = list_iterator_new(shape->cells, LIST_TAIL);

  while ((node = list_iterator_next(it))) {
    Point cellPos = Point_Add(pos, node->val);

    // Sanity check, disallow edge-to-edge contact of the same color
    if (!Board_HasCell(board, &cellPos) ||
        Board_IsCellOccupied(board, &cellPos) ||
        Board_CellHasInvalidEdgeToEdgeContact(board, &cellPos, color)) {
      list_iterator_destroy(it);
      return false;
    }

    // TODO: Edit condition to support games with 2 players
    if (Board_IsCornerCell(board, &cellPos)) isInCorner = true;

    // Corner-to-corner contact shall be present between at least two cells of
    // the same color
    if (Board_CellHasValidCornerToCornerContact(board, &cellPos, color)) {
      hasValidCornerToCornerContact = true;
    }
  }

  list_iterator_destroy(it);

  return hasValidCornerToCornerContact || isInCorner;
}

Point Board_GetShapeMaxPos(Board *self, Shape *shape) {
  return (Point){self->width - shape->width, self->height - shape->height};
}

Point Board_GetOverlayCellPos(Board *self, Point *mousePos) {
  double cellWidth = (double)self->display_rect->w / self->width;
  double cellHeight = (double)self->display_rect->h / self->height;

  return (Point){floor((mousePos->x - self->display_rect->x) / cellWidth),
                 floor((mousePos->y - self->display_rect->y) / cellHeight)};
}

Point Board_NormalizeOverlayCellPos(Board *self, Point *pos) {
  if (!self->overlay_shape) return *pos;

  Point overlay_shape_max_pos = Board_GetShapeMaxPos(self, self->overlay_shape);
  return Point_Min(pos, &overlay_shape_max_pos);
}

void Board_PlaceShape(Board *board, Point *pos, Color color, Shape *shape) {
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(shape->cells, LIST_TAIL);

  while ((node = list_iterator_next(it))) {
    Point cellPos = Point_Add(pos, node->val);
    board->cells[cellPos.x][cellPos.y] = color;
  }

  list_iterator_destroy(it);
}

void Board_RenderDrawCell(Board *board, Point *pos, Color *color, int layer) {
  // Sanity check
  if (!Board_HasCell(board, pos)) return;

  SDL_SetRenderTarget(board->renderer, board->textures[layer]);

  int cellWidth = board->display_rect->w / board->width;
  int cellHeight = board->display_rect->h / board->height;

  SDL_Rect target_rect = {pos->x * cellWidth, pos->y * cellHeight, cellWidth,
                          cellHeight};

  // Draw cell
  SDL_Color cell_color =
      kShapeCellColors[color ? *color : board->cells[pos->x][pos->y]];

  SDL_SetRenderDrawColor(board->renderer, cell_color.r, cell_color.g,
                         cell_color.b, cell_color.a);
  SDL_RenderFillRect(board->renderer, &target_rect);

  // Draw border if necessary
  if (layer == 0) {
    SDL_SetRenderDrawColor(board->renderer, kShapeCellBorderColor.r,
                           kShapeCellBorderColor.g, kShapeCellBorderColor.b,
                           kShapeCellBorderColor.a);
    SDL_RenderDrawRect(board->renderer, &target_rect);
  }
}

void Board_RenderDrawShape(Board *board, Point *pos, Color *color, Shape *shape,
                           int layer) {
  list_node_t *node;
  list_iterator_t *it = list_iterator_new(shape->cells, LIST_TAIL);

  while ((node = list_iterator_next(it))) {
    Point cellPos = Point_Add(pos, node->val);
    Board_RenderDrawCell(board, &cellPos, color, layer);
  }

  list_iterator_destroy(it);
}

void Board_RenderUpdateOverlay(Board *self, Point *overlay_cell_pos) {
  // Clear previous overlay if necessary
  if (self->overlay_shape) {
    SDL_SetRenderTarget(self->renderer, self->textures[1]);
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(self->renderer);
  }

  // Sanity check
  if (!overlay_cell_pos) return;

  // Hide overlay and freeze the board if the game has ended
  if (self->game->has_ended) {
    SDL_FreeCursor(SDL_GetCursor());
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    self->is_frozen = true;
    return;
  }

  // Update board properties
  Player *current_player = Game_GetCurrentPlayer(self->game);
  Shape *overlay_shape = Player_GetSelectedShape(current_player);
  self->overlay_cell_pos = *overlay_cell_pos;
  self->overlay_shape = overlay_shape;

  Point normalized_overlay_cell_pos =
      Board_NormalizeOverlayCellPos(self, overlay_cell_pos);

  // Draw new overlay
  Board_RenderDrawShape(self, &normalized_overlay_cell_pos,
                        &current_player->color, overlay_shape, 1);

  // Check whether the shape is placeable and set cursor according to that
  if (Board_IsShapePlaceable(self, &normalized_overlay_cell_pos,
                             current_player->color, overlay_shape)) {
    SDL_FreeCursor(SDL_GetCursor());
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
  } else {
    SDL_FreeCursor(SDL_GetCursor());
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO));
  }
}

void Board_RenderUpdateAll(Board *self) {
  // Update all cells
  for (int x = 0; x < self->width; ++x) {
    for (int y = 0; y < self->height; ++y) {
      Point cellPos = {x, y};
      Board_RenderDrawCell(self, &cellPos, NULL, 0);
    }
  }

  // Update overlay
  Board_RenderUpdateOverlay(self, &self->overlay_cell_pos);
}

void Board_RenderCopy(Board *board) {
  for (int i = 0; i < kBoardNumLayers; ++i) {
    SDL_RenderCopy(board->renderer, board->textures[i], NULL,
                   board->display_rect);
  }
}

// Bool indicates whether a repaint is necessary
bool Board_HandleEvent(Board *self, SDL_Event *e) {
  if (e->type == SDL_WINDOWEVENT &&
      e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
    Board_RenderUpdateAll(self);
    return true;
  }

  if (self->is_frozen) return false;

  switch (e->type) {
    case SDL_MOUSEMOTION: {
      Point mouse_pos = {e->motion.x, e->motion.y};
      if (!SDL_PointInRect((SDL_Point *)&mouse_pos, self->display_rect)) {
        // Restore cursor
        SDL_FreeCursor(SDL_GetCursor());
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
        return false;
      }

      Point overlay_cell_pos = Board_GetOverlayCellPos(self, &mouse_pos);

      // Don't update the overlay needlessly
      if (Point_Equals(&overlay_cell_pos, &self->overlay_cell_pos)) {
        return false;
      }

      Board_RenderUpdateOverlay(self, &overlay_cell_pos);

      return true;
    }

    case SDL_MOUSEBUTTONUP: {
      if (e->button.button != SDL_BUTTON_LEFT) return false;

      Point mouse_pos = {e->button.x, e->button.y};
      if (!SDL_PointInRect((SDL_Point *)&mouse_pos, self->display_rect))
        return false;

      Point overlay_cell_pos = Board_GetOverlayCellPos(self, &mouse_pos);
      Point normalized_overlay_cell_pos =
          Board_NormalizeOverlayCellPos(self, &overlay_cell_pos);

      Player *current_player = Game_GetCurrentPlayer(self->game);
      Shape *selected_shape = Player_GetSelectedShape(current_player);

      if (!Board_IsShapePlaceable(self, &normalized_overlay_cell_pos,
                                  current_player->color, selected_shape)) {
        return false;
      }

      Board_PlaceShape(self, &normalized_overlay_cell_pos,
                       current_player->color, selected_shape);

      Board_RenderDrawShape(self, &normalized_overlay_cell_pos,
                            &current_player->color, selected_shape, 0);

      list_node_t *placed_shape_node = current_player->selected_shape_node;
      Shape *placed_shape = placed_shape_node->val;

      // Calculate new score of the player
      current_player->score += placed_shape->cells->len;
      if (current_player->score == 0) {
        // Bonus: +20 points if the last shape played was a monomino, +15 points
        // otherwise
        current_player->score += placed_shape->cells->len == 1 ? 20 : 15;
      }

      Player_NextShape(current_player);
      list_remove(current_player->placeable_shapes, placed_shape_node);

      Game_NextPlayer(self->game, false);
      Board_RenderUpdateOverlay(self, &overlay_cell_pos);

      return true;
    }

    case SDL_KEYDOWN: {
      switch (e->key.keysym.sym) {
        case SDLK_LEFT:
        case SDLK_a:
        case SDLK_RIGHT:
        case SDLK_d:
        case SDLK_UP:
        case SDLK_w:
        case SDLK_DOWN:
        case SDLK_s:
        case SDLK_SPACE:
        case SDLK_p:
          Board_RenderUpdateOverlay(self, &self->overlay_cell_pos);
          return true;
      }
    }
  }

  return false;
}
