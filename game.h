#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#define NL 12 // nombre de cercles par colonne
#define NC 12 // nombre de cercles par lignes
              // Ces paramètres devraient toujours être multiples de 4
#define ML NL // nombre de lignes de la matrice
#define MC NC+1 // nombre de colonnes de la matrice
#define MAX_RANDOM_REMOVED_POS (NL+NC)
#define MIN_RANDOM_REMOVED_POS 3
#define POS_IN_MAP(x, y) ( ((x) >= 0) && ((y) >= 0) && ((x) < NL) && ((y) < NC) )

typedef struct
{
    int x;
    int y;
}Position;

typedef enum {FREE_POSITION, REMOVED_POSITION, CAT_POSITION} positionType;
typedef positionType GameMap[ML][MC];
typedef enum {CAT_WIN, CAT_LOSE} GameOutput;
typedef enum {LEFT = 4, RIGHT = 6, TOP_LEFT = 7, TOP_RIGHT = 9, BOT_LEFT = 1, BOT_RIGHT = 3} Movement;
void initMap(GameMap m);
void showMap(GameMap m);
GameOutput game(GameMap m);
void moveCat(Position *catPos, Movement movement, GameMap map);
#endif // GAME_H_INCLUDED
