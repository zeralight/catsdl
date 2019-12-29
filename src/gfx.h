#ifndef GFX_H_INCLUDED
#define GFX_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "common.h"
#include "game.h"
#include "map.h"
#include "scores.h"
#define W_WINDOW 800
#define H_WINDOW 680
#define X_START 50
#define X_END 750
#define Y_START 30
#define Y_END H_WINDOW - 30
#define X_OFFSET 10
#define Y_OFFSET 10
#define MAIN_FONT_SIZE 28

typedef struct {
  int x;
  int y;
} pos2D;

void gfxInit();
void gfxRelease();
void renderMainMenu(SDL_Rect *png, SDL_Rect *pts, SDL_Rect *po, SDL_Rect *pq);
void renderMap(const gameMap *map, const position *catPos);
void loadGameMenuTx(texture *diff, texture *size, texture *dp, texture *dm, texture *sp,
                    texture *sm, texture *start, texture *back);
void renderGameMenu(texture *diff, texture *size, texture *dp, texture *dm, texture *sp,
                    texture *sm, texture *start, texture *back, const gameLevel *lvl);
void renderOutput(bool win, char name[]);
void renderScores(SDL_Rect *backRect, const scVec *ss, size_t max);
bool pointInRect(const pos2D *point, const SDL_Rect *rect);
pos2D pos2DFromPos(const position *pos);
position posFromPos2D(const pos2D *pos, bool *exist, size_t mapSize);

#endif  // GFX_H_INCLUDED
