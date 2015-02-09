#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "game.h"


int main()
{
    GameMap map;

    initMap(map);
    GameOutput out = game(map);
    if(out == CAT_WIN)
        puts("le chat a gagne");
    else
        puts("le chat a perdu");

    return 0;
}
