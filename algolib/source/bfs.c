#include <stdlib.h>

#include "bfs.h"
#include "graph.h"
#include "list.h"
#include "queue.h"

int bfs(Graph *graph, BfsVertex *start, List *hops) {

Queue              queue;

AdjList            *adjlist,
                   *clr_adjlist;

BfsVertex          *clr_vertex,
                   *adj_vertex;

ListElmt           *element,
                   *member;

/* Initializarea nodurilor din graf (we call it vertex like in 3D programming point) */

for (element = list_head(&graph_adjlists(graph)); element != NULL; element =
   list_next(element)) {

   clr_vertex = ((AdjList *)list_data(element))->vertex;

   if (graph->match(clr_vertex, start)) {

      /* Initializarea nodului de start*/

      clr_vertex->color = gray;
      clr_vertex->hops = 0;

      }

   else {

      /*Initializarea celorlalte noduri*/

      clr_vertex->color = white;
      clr_vertex->hops = -1;

   }

}

/* Initializare coada cu lista de adiacenta a nodului de start */

queue_init(&queue, NULL);

if (graph_adjlist(graph, start, &clr_adjlist) != 0) {

   queue_destroy(&queue);
   return -1;

}

if (queue_enqueue(&queue, clr_adjlist) != 0) {

   queue_destroy(&queue);
   return -1;

}

/* BFS  */

while (queue_size(&queue) > 0) {

   adjlist = queue_peek(&queue);

   /* se traverseaza lista de adiacente */

   for (member = list_head(&adjlist->adjacent); member != NULL; member =
      list_next(member)) {

      adj_vertex = list_data(member);

      if (graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0) {

         queue_destroy(&queue);
         return -1;

      }

      clr_vertex = clr_adjlist->vertex;

      if (clr_vertex->color == white) {

         clr_vertex->color = gray;
         clr_vertex->hops = ((BfsVertex *)adjlist->vertex)->hops + 1;

         if (queue_enqueue(&queue, clr_adjlist) != 0) {

            queue_destroy(&queue);
            return -1;

         }

      }

   }

   if (queue_dequeue(&queue, (void **)&adjlist) == 0) {

      ((BfsVertex *)adjlist->vertex)->color = black;

      }

   else {

      queue_destroy(&queue);
      return -1;

   }

}

queue_destroy(&queue);

/* Initializare lista cu salturi */

list_init(hops, NULL);

for (element = list_head(&graph_adjlists(graph)); element != NULL; element =
   list_next(element)) {

   /* Sarim peste nodurile nevizitate (hops = -1) */

   clr_vertex = ((AdjList *)list_data(element))->vertex;

   if (clr_vertex->hops != -1) {

      if (list_ins_next(hops, list_tail(hops), clr_vertex) != 0) {

         list_destroy(hops);
         return -1;

      }

   }

}

return 0;

}
