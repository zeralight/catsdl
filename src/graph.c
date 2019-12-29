#include "graph.h"
#include "game.h"
static void swapVertices(vertex *a, vertex *b) {
  vertex tmp = *a;
  *a = *b;
  *b = tmp;
}

gameGraph *createGraph(const gameMap *map) {
  gameGraph *graph = malloc(sizeof *graph);
  if (graph == NULL) return NULL;

  graph->size = map->size * map->size;
  graph->vertices = malloc(graph->size * sizeof *graph->vertices);
  if (graph->vertices == NULL) {
    free(graph);
    return NULL;
  }

  size_t i;
  position p = {0, 0};
  for (i = 0; i < graph->size; i++) {
    graph->vertices[i].pos = p;
    graph->vertices[i].type = map->mat[p.i][p.j];
    advancePos(&p, map->size);
  }
  return graph;
}

void updateGraph(gameGraph *graph, const position *pos) {
  int i;
  for (i = 0; i < graph->size && memcmp(&graph->vertices[i], pos, sizeof *pos) != 0; i++)
    ;  // A REVOIR !!!!!!!!!!!!!!!!!!!
  assert(i < graph->size);
  graph->vertices[i].type = MARKED;
}

void graphDestroy(gameGraph *graph) {
  free(graph->vertices);
  free(graph);
}

void dijkstra(gameGraph *graph, const position *src) {
  size_t i;
  for (i = 0; i < graph->size && memcmp(&graph->vertices[i].pos, src, sizeof *src) != 0;
       i++)  // Supprimer i < graph->size
    graph->vertices[i].dist = -1;

  assert(i < graph->size);
  graph->vertices[i].dist = 0;
  graph->vertices[i].reachedFrom = NULL;
  for (++i; i < graph->size; i++) graph->vertices[i].dist = -1;

  bool allVisited = false;
  for (i = 0; !allVisited; i++) {
    size_t j;
    vertex *min = NULL;

    for (j = i; j < graph->size; j++)
      if (graph->vertices[j].dist > -1 && (min == NULL || min->dist > graph->vertices[j].dist))
        min = &graph->vertices[j];
    if (min != NULL) {
      swapVertices(min, &graph->vertices[i]);
      min = &graph->vertices[i];
      for (j = i + 1; j < graph->size; j++) {
        if (neighbors(&min->pos, &graph->vertices[j].pos) && graph->vertices[j].type == UNMARKED &&
            (graph->vertices[j].dist == -1 || min->dist + 1 < graph->vertices[j].dist)) {
          graph->vertices[j].dist = min->dist + 1;
          graph->vertices[j].reachedFrom = min;
        }
      }
    } else
      allVisited = true;
  }
}
