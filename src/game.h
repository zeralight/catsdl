#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <assert.h>
#include <stdio.h>
#include "common.h"
#include "gfx.h"
#include "graph.h"

int xStart;
int yStart;
gameLevel gameMenu(bool *back);
void advancePos(position *pos, unsigned lim);
const position *nextAction(gameGraph *graph, const position *catPos, const gameMap *map);
bool posLim(const position *p, size_t lim);
#endif  // GAME_H_INCLUDED
