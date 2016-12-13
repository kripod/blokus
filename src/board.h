#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "color.h"
#include "game.h"
#include "player.h"
#include "point.h"
#include "shape.h"

/**
 * A board which corresponds to a Game.
 */
typedef struct Board {
  /**
   * Game which the board was created for.
   */
  Game *game;

  /**
   * Width of the board (measured in cells).
   */
  int width;

  /**
   * Height of the board (measured in cells).
   */
  int height;

  /**
   * Color of each cell, represented as a 2D dynamic array.
   */
  Color **cells;

  /**
   * True whether user interaction is not enabled.
   */
  bool is_frozen;

  /**
   * Renderer used for drawing on the textures of self.
   */
  SDL_Renderer *renderer;

  /**
   * Display rectangle for the entire rendering target.
   */
  const SDL_Rect *display_rect;

  /**
   * Textures which store display information.
   */
  SDL_Texture **textures;

  /**
   * Starting position of the cell(s) currently drawn on the overlay.
   */
  Point overlay_cell_pos;

  /**
   * Shape which is currently drawn on the overlay.
   */
  Shape *overlay_shape;
} Board;

Board Board_Create(Game *game, int width, int height, SDL_Renderer *renderer,
                   const SDL_Rect *display_rect);
void Board_Destroy(Board *self);

bool Board_HasCell(Board *self, Point *pos);
bool Board_IsCellOccupied(Board *board, Point *pos);
bool Board_IsCornerCell(Board *board, Point *pos);

bool Board_CellHasValidCornerToCornerContact(Board *board, Point *pos,
                                             Color color);

bool Board_CellHasInvalidEdgeToEdgeContact(Board *board, Point *pos,
                                           Color color);

bool Board_IsShapePlaceable(Board *board, Point *pos, Color color,
                            Shape *shape);

Point Board_GetShapeMaxPos(Board *self, Shape *shape);
Point Board_GetOverlayCellPos(Board *self, Point *mousePos);
Point Board_NormalizeOverlayCellPos(Board *self, Point *pos);

void Board_PlaceShape(Board *board, Point *pos, Color color, Shape *shape);

void Board_RenderDrawCell(Board *board, Point *pos, Color *color, int layer);
void Board_RenderDrawShape(Board *board, Point *pos, Color *color, Shape *shape,
                           int layer);
void Board_RenderUpdateOverlay(Board *self, Point *overlay_cell_pos);
void Board_RenderUpdateAll(Board *self);
void Board_RenderCopy(Board *board);

bool Board_HandleEvent(Board *self, SDL_Event *e);

#endif  // BOARD_H_INCLUDED
