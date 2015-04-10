#include "map.h"

gameMap *mapCreate(const gameLevel *lvl)
{
    gameMap *map = malloc(sizeof *map);
    if(map == NULL){
        perror("Memory Error : ");
        exit(EXIT_FAILURE);
    }
    map->size = lvl->size;
    map->mat = malloc(sizeof *map->mat * map->size);
    if(map->mat == NULL){
        perror("Memory Error : ");
        exit(EXIT_FAILURE);
    }
    size_t i;
    for(i = 0; i < map->size; i++)
        if((map->mat[i] = malloc(sizeof *map->mat[i] * map->size)) == NULL){
            perror("Memory Error : ");
            exit(EXIT_FAILURE);
        }

    for(i = 0; i < map->size; i++){
        size_t j;
        for(j = 0; j < map->size; j++){
            map->mat[i][j] = UNMARKED;
        }
    }

    unsigned maxNbMarked, minNbMarked;
    if(lvl->diff == HARD){
        maxNbMarked = map->size;
        minNbMarked = 0;
    }
    else if(lvl->diff == MEDIUM){
        maxNbMarked = 2*map->size;
        minNbMarked = map->size;
    }
    else{
        maxNbMarked = 3*map->size;
        minNbMarked = 2*map->size;
    }

    unsigned nbMarked = rand() % (maxNbMarked - minNbMarked + 1) + minNbMarked;

    unsigned k;
    for(k = 0; k < nbMarked; k++){
        unsigned i, j;
        do{
            i = rand() % map->size;
            j = rand() % map->size;
        }while(map->mat[i][j] == MARKED || (i == map->size/2 && j == map->size/2));
        map->mat[i][j] = MARKED;
    }

    return map;
}

void mapDestroy(gameMap *map)
{
    int i;
    for(i = 0; i < map->size; i++)
            free(map->mat[i]);
    free(map->mat);
    free(map);
}
