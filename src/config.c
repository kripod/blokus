#include <SDL2/SDL.h>
#include "config.h"

const char kFontMainPath[] = "resources/Roboto-Regular.ttf";
const char kShapesPath[] = "resources/shapes.txt";

const char kWindowTitle[] = "Blokus";
const int kWindowDefaultWidth = 640;
const int kWindowDefaultHeight = 480;
const int kWindowLogicalWidth = 523;
const int kWindowLogicalHeight = 480;
const int kWindowFramerate = 60;
const SDL_Color kWindowBgColor = {0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE};

const int kFontMainSize = 24;

const int kBoardNumLayers = 2;
const SDL_Rect kBoardDisplayRect = {10, 10, 460, 460};
const int kBoardOverlayAlpha = SDL_ALPHA_OPAQUE / 2;

const SDL_Color kShapeCellColors[] = {
    {0xec, 0xf0, 0xf1, SDL_ALPHA_OPAQUE},  // None
    {0x34, 0x98, 0xdb, SDL_ALPHA_OPAQUE},  // Blue
    {0xf1, 0xc4, 0x0f, SDL_ALPHA_OPAQUE},  // Yellow
    {0xe7, 0x4c, 0x3c, SDL_ALPHA_OPAQUE},  // Red
    {0x2e, 0xcc, 0x71, SDL_ALPHA_OPAQUE},  // Green
};
const SDL_Color kShapeCellBorderColor = {0xbd, 0xc3, 0xc7, SDL_ALPHA_OPAQUE};

const SDL_Point kScoreboardDisplayStartPoint = {480, 10};
const int kScoreboardRowPadding = 2;
