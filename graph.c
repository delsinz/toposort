/*
 * ALGORITHMS ARE FUN!
 * Name: Mingyang Zhang
 * Student ID: 650242
*/
#include <stdlib.h>
#include <assert.h>

#include "graph.h"


/* Returns a pointer to a new graph with order vertices */
Graph new_graph(int order) {
    Vertex vertex_array = malloc(sizeof(struct vertex_t) * order);
    Graph graph = malloc(sizeof(struct graph_t));
    graph->order = order;
    graph->size = 0;
    graph->vertices = vertex_array;
    return graph;
}

/* Returns whether aim and vertex are pointing to the same location */
bool ptr_eq(void *aim, void *vertex) {
    if(aim == vertex){
        return true;
    }
    return false;
}

/* Returns whether aim and vertex have the same id */
bool id_eq(void *aim, void *vertex) {
    if(((Vertex)aim)->id == ((Vertex)vertex)->id){
        return true;
    }
    return false;
}

/* Add the edge from v1 to v2 to graph */
void add_edge(Graph graph, int v1, int v2) {
    /* Update number of edges */
    graph->size += 1;
    /* Get v1 and v2 pointers */
    int i;
    Vertex v_out, v_in;
    Vertex ptr = graph->vertices;
    for(i = 0; i < graph->order; i++, ptr++){
        if(ptr->id == v1){
            v_out = ptr;
        } else if(ptr->id == v2){
            v_in = ptr;
        }
    }
    /* Update v1 and v2 */
    if(v_out->out){
        insert(v_in, &(v_out->out));
    } else{
        v_out->out = push(NULL, v_in);
    }
    if(v_in->in){
        insert(v_out, &(v_in->in));
    } else{
        v_in->in = push(NULL, v_out);
    }
}

/* Free the memory allocated to graph */
void free_graph(Graph graph) {
    free(graph->vertices);
    graph->vertices = NULL;
    free(graph);
    graph = NULL;
}
