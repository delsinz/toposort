/*
 * ALGORITHMS ARE FUN!
 * Name: Mingyang Zhang
 * Student ID: 650242
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "toposort.h"

#define UNMARKED 0
#define TEMP -1
#define PERM 1

void visit(Vertex vertex, List* sorted, int mark[]);

/* Returns a list of topologically sorted vertices using the DFS method */
List dfs_sort(Graph graph) {
    List sorted = NULL;
    int order = graph->order;
    /* Construct mark array */
    int i;
    int mark[order];
    for(i = 0; i < order; i++){
        mark[i] = UNMARKED;
    }

    Vertex vptr = graph->vertices;
    for(i = 0; i < order; i++, vptr++){
        if(mark[vptr->id] == UNMARKED){
            visit(vptr, &sorted, mark);
        }
    }

    return sorted;
}

void
visit(Vertex vertex, List* sorted, int mark[]){
    int id = vertex->id;
    /* If n has temp mark */
    if(mark[id] == TEMP){
        exit(0);
    }

    /* If n not marked */
    if(mark[id] == UNMARKED){
        mark[id] = TEMP;
        List out = vertex->out;
        /* Visit all adjacent nodes */
        while(out){
            visit((Vertex)(out->data), sorted, mark);
            out = out->next;
        }
        mark[id] = PERM;
        /* Add n to head of list */
        if(*sorted){
            prepend(sorted, vertex);
        } else{
            *sorted = push(NULL, vertex);
        }
    }
}

/* Returns a list of topologically sorted vertices using the Kahn method */
List kahn_sort(Graph graph) {
    List sorted = NULL, source = NULL;

    /* Get all source nodes */
    int i;
    int order = graph->order;
    Vertex vptr = graph->vertices;
    for(i = 0; i < order; i++, vptr++){
        if(!vptr->in){
            if(source){
                insert(vptr, &source);
            } else{
                source = push(NULL, vptr);
            }
        }
    }

    /* Khan!!! -- Spock */
    /* Start sorting */
    while(source){
        Vertex v_out = pop(&source);
        if(sorted){
            insert(v_out, &sorted);
        } else{
            sorted = push(NULL, v_out);
        }
        /* For each node v_in from v_out */
        List out = v_out->out;
        while(out){
            /* Remove edge */
            Vertex v_in = pop(&out);
            del(&id_eq, v_out, &(v_in->in));
            /* If v_in has no incoming edges */
            if(v_in->in == NULL){
                if(source){
                    insert(v_in, &source);
                } else{
                    source = push(NULL, v_in);
                }
            }
        }
    }
    return sorted;
}

/* Uses graph to verify vertices are topologically sorted */
bool verify(Graph graph, List vertices) {
    int i;
    int order = graph->order;

    /* If order does not match length of list */
    if(order != len(vertices)){
        return false;
    }

    /* Build array whose index is id and value is position in sorted list */
    int id;
    int pos_array[order];
    for(i = 0; i < order; i++){
        id = ((Vertex)(vertices->data))->id;
        pos_array[id] = i;
        vertices = vertices->next;
    }

    /* Pos of each node in the out list of node n must be after pos of n */
    Vertex v_out = graph->vertices;
    for(i = 0; i < order; i++, v_out++){
        List out = v_out->out;
        while(out){
            Vertex v_in = out->data;
            if(pos_array[v_out->id] >= pos_array[v_in->id]){
                return false;
            }
            out = out->next;
        }
    }
    return true;
}
