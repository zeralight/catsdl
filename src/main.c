#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "gfx.h"
#include "graph.h"
#include "map.h"
#include "scores.h"

typedef enum { NEW_GAME = 1, SCORES = 2, OPTIONS = 3, QUIT = 4 } menuChoice;

static void init();
static void release();
static int mainMenu();
static void scores();
static void play();

int main(int argc, char **argv) {
  init();

  menuChoice choice;
  do {
    choice = mainMenu();
    switch (choice) {
      case NEW_GAME:
        play();
        break;
      case SCORES:
        scores();
        break;
      case OPTIONS:
        break;
      case QUIT:
        break;
    }
  } while (choice != QUIT);

  release();

  return EXIT_SUCCESS;
}

void init() {
  srand(time(NULL));
  FILE *f = fopen("scores", "rb");
  if (f == NULL) {
    f = fopen("scores", "wb");
    if (f == NULL) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create scores", NULL);
      exit(EXIT_FAILURE);
    }
  }
  fclose(f);

  gfxInit();
}

void release() { gfxRelease(); }

int cmp(const void *a, const void *b) {
  return -(((scoreField *)a)->val - (((scoreField *)b)->val));
}

void scores() {
  scVec *v = scLoad();

  qsort(v->elements, v->size, sizeof *v->elements, cmp);
  SDL_Rect br;
  renderScores(&br, v, 5);

  while (true) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT)
      exit(EXIT_SUCCESS);
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
      pos2D p = {e.button.x, e.button.y};
      if (pointInRect(&p, &br)) break;
    }
  }
  scRelease(v);
}

int mainMenu() {
  menuChoice res;

  SDL_Rect png, pts, po, pq;
  renderMainMenu(&png, &pts, &po, &pq);
  while (true) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      res = QUIT;
      break;
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
      pos2D p = {e.button.x, e.button.y};
      if (pointInRect(&p, &png)) {
        res = NEW_GAME;
        break;
      } else if (pointInRect(&p, &pts)) {
        res = SCORES;
        break;
      } else if (pointInRect(&p, &po)) {
        res = OPTIONS;
        break;
      } else if (pointInRect(&p, &pq)) {
        res = QUIT;
        break;
      }
    }
  }
  return res;
}

static void play() {
  bool back;
  gameLevel dif = gameMenu(&back);
  if (back) return;
  gameMap *map = mapCreate(&dif);
  gameGraph *graph = createGraph(map);
  position catPos = {map->size / 2, map->size / 2};

  xStart = X_START +
           (X_END - X_START - (X_OFFSET * (map->size - 1) + unmarkedpi->rect.w * (map->size))) / 2;
  yStart = Y_START +
           (Y_END - Y_START - (Y_OFFSET * (map->size - 1) + unmarkedpi->rect.h * (map->size))) / 2;

  scoreField sc;
  sc.val = 100 * (map->size * map->size);
  position p;

  bool win = false, lose = false;
  while (true) {
    renderMap(map, &catPos);
    if (win || lose) break;

    SDL_Event e;
    while (true) {
      SDL_WaitEvent(&e);
      if (e.type == SDL_QUIT)
        exit(EXIT_SUCCESS);
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        pos2D p2d = {e.button.x, e.button.y};
        bool exist;
        p = posFromPos2D(&p2d, &exist, map->size);
        if (exist && map->mat[p.i][p.j] == UNMARKED && memcmp(&p, &catPos, sizeof p) != 0) break;
      }
    }
    map->mat[p.i][p.j] = MARKED;
    sc.val -= 100;
    updateGraph(graph, &p);

    const position *next = nextAction(graph, &catPos, map);
    if (next == NULL)
      win = true;
    else {
      if (posLim(next, map->size)) lose = true;
      memcpy(&catPos, next, sizeof catPos);
    }
  }

  if (win) {
    int p;
    switch (dif.diff) {
      case HARD:
        p = 5;
        break;
      case MEDIUM:
        p = 3;
        break;
      case EASY:
        p = 1;
        break;
    }
    sc.val += 100 * p;
  }

  renderOutput(win, sc.name);
  addScore(&sc);
  mapDestroy(map);
  graphDestroy(graph);
}
