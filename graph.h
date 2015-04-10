#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "common.h"
#include "map.h"
struct vertex
{
    position pos;
    place type;
    int dist;
    struct vertex *reachedFrom;
};

typedef struct vertex vertex;

typedef struct
{
    vertex *vertices;
    size_t size;
}gameGraph;

gameGraph *createGraph(const gameMap *map);
void updateGraph(gameGraph *graph, const position *pos);
void graphDestroy(gameGraph *graph);
void dijkstra(gameGraph *graph, const position *src);

#endif // GRAPH_H_INCLUDED
