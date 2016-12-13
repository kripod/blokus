#include <list.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "color.h"
#include "config.h"
#include "list_utils.h"
#include "player.h"
#include "point.h"
#include "shape.h"

int main(int argc, char **argv) {
  // Supress warnings about unused variables
  (void)argc;
  (void)argv;

  // Load shapes from file
  FILE *stream = fopen(kShapesPath, "rt");
  if (!stream) {
    perror("Could not load shapes from file");
    return EXIT_FAILURE;
  }
  list_t *shapes = File_ReadShapes(stream);
  fclose(stream);

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
    list_destroy(shapes);
    fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  // Set render hinting and then initialize main window
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
  SDL_Window *window = SDL_CreateWindow(
      kWindowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      kWindowDefaultWidth, kWindowDefaultHeight,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_SetWindowMinimumSize(window, kWindowDefaultWidth / 2,
                           kWindowDefaultHeight / 2);

  // Initialize main renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                      SDL_RENDERER_TARGETTEXTURE);
  SDL_RenderSetLogicalSize(renderer, kWindowLogicalWidth, kWindowLogicalHeight);

  // Load main font
  TTF_Font *fontMain = TTF_OpenFont(kFontMainPath, kFontMainSize);

  // TODO: Add a menu

  // Set up a new game
  int num_players = 4;
  Game game = Game_Create(num_players, shapes);
  Board board = Board_Create(&game, 20, 20, renderer, &kBoardDisplayRect);

  bool quit = false;
  bool render_screen = true;

  // Main loop
  do {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0 && !quit) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }

        case SDL_WINDOWEVENT:
          render_screen = true;

        default: {
          render_screen |=
              Game_HandleEvent(&game, &e) | Board_HandleEvent(&board, &e);
          break;
        }
      }
    }

    if (render_screen) {
      SDL_SetRenderTarget(renderer, NULL);

      // Clear screen
      SDL_SetRenderDrawColor(renderer, kWindowBgColor.r, kWindowBgColor.g,
                             kWindowBgColor.b, kWindowBgColor.a);
      SDL_RenderClear(renderer);

      // Render each element of the screen
      Board_RenderCopy(&board);

      // Render scoreboard
      Player *playersArr[game.players->len];
      list_toarray(game.players, (void **)playersArr);
      qsort(playersArr, game.players->len, sizeof(Player *),
            Player_ComparePointers);

      for (int i = 0; i < (int)game.players->len; ++i) {
        Player *player = playersArr[i];

        char score_str[12];
        sprintf(score_str, "%d", player->score);
        SDL_Surface *score_surface = TTF_RenderUTF8_Blended(
            fontMain, score_str, kShapeCellColors[player->color]);
        SDL_Texture *score_texture =
            SDL_CreateTextureFromSurface(renderer, score_surface);

        SDL_Rect target_rect = {
            kScoreboardDisplayStartPoint.x,
            kScoreboardDisplayStartPoint.y +
                i * (score_surface->h + kScoreboardRowPadding),
            score_surface->w, score_surface->h};
        SDL_RenderCopy(renderer, score_texture, NULL, &target_rect);

        SDL_DestroyTexture(score_texture);
        SDL_FreeSurface(score_surface);
      }

      SDL_RenderPresent(renderer);

      render_screen = false;
    }

    // Reduce resource usage by letting the application sleep
    SDL_Delay(1000 / kWindowFramerate);
  } while (!quit);

  // Free up resources
  Board_Destroy(&board);
  Game_Destroy(&game);
  list_destroy(shapes);
  TTF_CloseFont(fontMain);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
