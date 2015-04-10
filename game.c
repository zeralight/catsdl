#include "game.h"

gameLevel gameMenu(bool *b)
{
    gameLevel res;
    texture diff, size, dp, dm, sp, sm, start, back;

    loadGameMenuTx(&diff, &size, &dp, &dm, &sp, &sm, &start, &back);
    res.size = 5;
    res.diff = EASY;
    *b = false;
    while(true){
        renderGameMenu(&diff, &size, &dp, &dm, &sp, &sm, &start, &back, &res);
        SDL_Event e;
        SDL_WaitEvent(&e);

        if(e.type == SDL_QUIT)
            exit(EXIT_SUCCESS);
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            pos2D p = {e.button.x, e.button.y};
            if(pointInRect(&p, &dp.rect) && res.diff < HARD){
                ++res.diff;
            }
            else if(pointInRect(&p, &dm.rect) && res.diff > EASY)
                --res.diff;
            else if(pointInRect(&p, &sp.rect) && res.size < 11)
                res.size += 2;
            else if(pointInRect(&p, &sm.rect) && res.size > 5)
                res.size -= 2;
            else if(pointInRect(&p, &start.rect))
                break;
            else if(pointInRect(&p, &back.rect)){
                *b = true;
                break;
            }
        }
    }

    return res;
}

void advancePos(position *pos, unsigned lim)
{
    ++pos->j;
    if(pos->j == lim){
        pos->j = 0;
        ++pos->i;
    }
}

bool posLim(const position *p, size_t lim)
{
    return (p->j == 0 || p->i == 0 || p->j+1 == lim || p->i+1 == lim);
}

const position *nextAction(gameGraph *graph, const position *catPos, const gameMap *map)
{
    dijkstra(graph, catPos);
    vertex *min = NULL;
    size_t i;
    for(i = 0; graph->vertices[i].dist > -1 && !posLim(&graph->vertices[i].pos, map->size); i++);
    if(graph->vertices[i].dist > -1)
            min = &graph->vertices[i];
        else
            return NULL;
    while(memcmp(&min->reachedFrom->pos, catPos, sizeof *catPos) != 0)
        min = min->reachedFrom;
    return &min->pos;
}

bool neighbors(const position *p1, const position *p2)
{
    if(p1->i == p2->i && (p1->j+1 == p2->j || p1->j == p2->j+1))
        return true;
    if(p1->j == p2->j && (p1->i+1 == p2->i || p1->i == p2->i+1))
        return true;
    if(p1->i % 2 == 0 && (p2->j+1 == p1->j && (p2->i+1 == p1->i || p2->i == p1->i+1)))
        return true;
    if(p1->i % 2 > 0 && (p1->j+1 == p2->j && (p2->i+1 == p1->i || p2->i == p1->i+1)))
        return true;

    return false;
}
