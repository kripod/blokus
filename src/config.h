#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <SDL2/SDL.h>

extern const char kFontMainPath[];
extern const char kShapesPath[];

extern const char kWindowTitle[];
extern const int kWindowDefaultWidth;
extern const int kWindowDefaultHeight;
extern const int kWindowLogicalWidth;
extern const int kWindowLogicalHeight;
extern const int kWindowFramerate;
extern const SDL_Color kWindowBgColor;

extern const int kFontMainSize;

extern const int kBoardNumLayers;
extern const SDL_Rect kBoardDisplayRect;
extern const int kBoardOverlayAlpha;

extern const SDL_Color kShapeCellColors[];
extern const SDL_Color kShapeCellBorderColor;

extern const SDL_Point kScoreboardDisplayStartPoint;
extern const int kScoreboardRowPadding;

#endif  // CONFIG_H_INCLUDED
