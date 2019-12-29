#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
typedef enum { MARKED, UNMARKED } place;
typedef struct {
  size_t size;
  place **mat;
} gameMap;

gameMap *mapCreate(const gameLevel *lvl);
void mapDestroy(gameMap *map);
bool neighbors(const position *p1, const position *p2);
#endif  // GAME_H_INCLUDED
