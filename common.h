#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    int i;
    int j;
}position;

enum {EASY = 0, MEDIUM = 1, HARD = 2} difficulty;

typedef struct
{
    int diff;
    unsigned size;
}gameLevel;

typedef struct
{
    SDL_Texture *tx;
    SDL_Rect rect;
}texture;

SDL_Renderer *renderer;
SDL_Window *window;
texture *bgi; //backGroundImage
texture *markedpi; // markedPlaceImage
texture *unmarkedpi; //UnmarkedPlaceImage
texture *cati;
TTF_Font *mainFont;
#endif // COMMON_H_INCLUDED
