#include "game.h"

void initMap(GameMap map)
{

    int i, j;

    for(i = 0; i < ML; i++)
        for(j = 0; j < MC; j++)
            map[i][j] = FREE_POSITION;

    srand(time(NULL));

    int n = rand() % (MAX_RANDOM_REMOVED_POS - MIN_RANDOM_REMOVED_POS + 1) + MIN_RANDOM_REMOVED_POS;

    int k;
    for(k = 0; k < n; k++){
        do{
            i = rand()%NL;
            j = rand()%NC;
        }while(((i >= NL/4) && (i < 3*NL/4) && (j >= NC/4) && (j < 3*NC/4))
               || (map[i][j] == REMOVED_POSITION));
            /*
               la première condition impose que les cercles enlevees au début ne doivent pas etre
               autour du centre pour laisser au chat un minimum de déplacement
            */
        map[i][j] = REMOVED_POSITION;
    }
    map[NL/2][NC/2] = CAT_POSITION;
}

void showMap(GameMap m)
{
    putchar('\n');

    int i, j;
    for(i = 0; i < ML; i++){
        printf("   ");
        for(j = 0; j < MC; j++){
            if(!(i%2 == 0 && j == MC-1) && !(i%2 > 0 && j == 0)){
                if(m[i][j] == FREE_POSITION)
                    putchar('+');
                else if(m[i][j] == REMOVED_POSITION)
                    putchar('-');
                else
                    putchar('*');

            }
                printf("  ");
        }
        putchar('\n');
    }
    putchar('\n');
}

static bool canMove(Position* p, GameMap m)
{
    if((p->x == 0) || (p->x == NL-1) || (p->y == 0) || (p->y == NC-1))
        return 1;

    return ((m[p->x-1][p->y] == FREE_POSITION) || (m[p->x-1][p->y+1] == FREE_POSITION)
            || (m[p->x][p->y-1] == FREE_POSITION) || (m[p->x][p->y+1] == FREE_POSITION)
            || (m[p->x+1][p->y] == FREE_POSITION) || (m[p->x+1][p->y+1] == FREE_POSITION));
}

GameOutput game(GameMap map)
{
    Position catPos = {NL/2, NC/2};
    Position p;
    do{
        showMap(map);
        printf("eliminez un cercle : ");
        scanf("%d%d", &(p.x), &(p.y));
        map[p.x][p.y] = REMOVED_POSITION;

        if(canMove(&catPos, map)){
            int movement;
            printf("deplacez le chat : ");
            scanf("%d", &movement);
            moveCat(&catPos, movement, map);
        }
    }while(POS_IN_MAP(catPos.x, catPos.y) && canMove(&catPos, map));


    return POS_IN_MAP(catPos.x, catPos.y);
}

void moveCat(Position *catPos, Movement movement, GameMap map)
{
    map[catPos->x][catPos->y] = FREE_POSITION;
    if(catPos->x%2 == 0){
            switch(movement){
            case TOP_LEFT: catPos->x--; break;
            case BOT_LEFT: catPos->x++; break;
            case TOP_RIGHT: catPos->x--; catPos->y++; break;
            case BOT_RIGHT: catPos->x++; catPos->y++; break;
            case LEFT: catPos->y--; break;
            case RIGHT: catPos->y++; break;
            }
    }
    else{
        switch(movement){
        case TOP_RIGHT: catPos->x--; break;
        case BOT_RIGHT: catPos->x++; break;
        case TOP_LEFT: catPos->x--; catPos->y--; break;
        case BOT_LEFT: catPos->x++; catPos->y--; break;
        case LEFT: catPos->y--; break;
        case RIGHT: catPos->y++; break;
        }
    }
    map[catPos->x][catPos->y] = CAT_POSITION;
}
