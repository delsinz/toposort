/*
 * ALGORITHMS ARE FUN!
 * Name: Mingyang Zhang
 * Student ID: 650242
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "graphio.h"

#define MAX_LINE_LEN 256

/* Loads the graph from input */
Graph load_graph(char *input) {
    char line[MAX_LINE_LEN + 1];
    FILE* fp = fopen(input, "r");

    /* Order */
    fgets(line, MAX_LINE_LEN, fp);
    int order = atoi(line);

    /* Construct graph */
    Graph graph = new_graph(order);
    Vertex vptr = graph->vertices;

    /* Labels & IDs */
    for(int i = 0; i < order; i++, vptr++){
        fgets(line, MAX_LINE_LEN, fp);
        strtok(line, "\n"); // Get rid of newline
        vptr->id = i;
        vptr->label = malloc(sizeof(line));
        strcpy(vptr->label, line);
    }

    /* Edges */
    int id_out, id_in;
    char* strint;
    while(fgets(line, MAX_LINE_LEN, fp) != NULL){
        strtok(line, "\n");
        strint = strtok(line, " ");
        id_out = atoi(strint);
        strint = strtok(NULL, " ");
        id_in = atoi(strint);
        add_edge(graph, id_out, id_in);
    }

    fclose(fp);

    return graph;

}

/* Prints the graph */
void print_graph(char *output, Graph graph) {
    int order = graph->order;
    Vertex vptr = graph->vertices;
    FILE* fp = fopen(output, "a");

    fprintf(fp, "digraph {\n");
    for(int i = 0; i < order; i++, vptr++){
        print_vertex_label(fp, vptr);
    }
    fprintf(fp, "}");

    fclose(fp);

}

/* Prints the destination vertex label surrounded by spaces */
void print_vertex_label(FILE *file, void *vertex) {
    fprintf(file, "    %s", ((Vertex)vertex)->label);
    List out = ((Vertex)vertex)->out;
    if(out){
        fprintf(file, " -> { ");
        while(out){
            fprintf(file, "%s ", ((Vertex)(out->data))->label);
            out = out->next;
        }
        fprintf(file, "}\n");
    } else{ // if no out edges
        fprintf(file, "\n");
    }

}

/* Prints the id of a vertex then a newline */
void print_vertex_id(FILE *file, void *vertex) {
    if (vertex)
        fprintf(file, "%d\n", ((Vertex)vertex)->id);
}

/* Returns a sequence of vertices read from file */
List load_vertex_sequence(FILE *file, Graph graph) {
    const char *err_duplicate = "Error: duplicate vertex %d %s\n";
    const char *err_order = "Error: graph order %d, loaded %d vertices\n";
    List list = NULL;
    int id;

    while(fscanf(file, "%d\n", &id) == 1) {
        assert(id >= 0);
        assert(id < graph->order);

        if (!insert_if(id_eq, graph->vertices + id, &list)) {
            fprintf(stderr, err_duplicate, id, graph->vertices[id].label);
            exit(EXIT_FAILURE);
        }
    }

    if (len(list) != graph->order) {
        fprintf(stderr, err_order, graph->order, len(list));
        exit(EXIT_FAILURE);
    }

    return list;
}
