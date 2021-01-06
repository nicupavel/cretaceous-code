#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include "list.h"

typedef struct BfsVertex_ {

void               *data;

VertexColor        color; 
int                hops; 

} BfsVertex;

int bfs(Graph *graph, BfsVertex *start, List *hops);

#endif
