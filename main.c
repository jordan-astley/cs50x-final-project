#include <stdio.h>
#include <stdlib.h>
#include "graph.c"
#include "djikstras.c"


int main(int argc, char* argv[])
{
    /* parse cmd line args
     $./main <input_graph.txt> <source vertex> */
    if (argc != 3)
    {
        printf("Error: Bad inputs.\n");
        printf("Usage: ./main <input_graph.txt> <source vertex>\n");
        return 1;
    }

    char* graph_filename = argv[1];
    FILE* graph_file = fopen(graph_filename, "r");
    if (graph_file == NULL)
    {
        printf("error: Could not open supplied graph file.\n");
        return 1;
    }

    int vertices = read_graph_vertices(graph_file);

    int source_vertex = atoi(argv[2]);
    if ((source_vertex > vertices - 1) || (source_vertex < 0)) {
        printf("Error: Supplied source vertex does not exist in graph\n");
        return 1;
    }


    // create a graph
    graph_t* graph = create_graph(vertices);
    if (graph == NULL)
    {
        printf("error: unable to allocate memory to graph struct\n");
        return 1;
    }

    // read data from disk and add edges to graph
    edge_data edge;
    while (read_edge(graph_file, &edge))
    {
        // printf("%i, %i, %i\n", edge.v1, edge.v2, edge.weight);
        add_edge(graph, edge.v1, edge.v2, edge.weight);
    }

    // print the graph
    print_graph(graph);

    // get an array of table entries to form the table
    table_entry* table = create_table(graph->vertices, source_vertex);
    if (table == NULL)
    {
        printf("error\n");
        return 1;
    }

    // run shortest path algorithm
    djikstra(table, graph);

    // print shortest path from source to each vertex
    for (int i = 0; i < graph->vertices; i++)
    {
        // print each vertex, its shortest distance from source and the path on new line
        printf("Vertex %i: %i\n", i, table[i].dis);
        int path_var = i;
        while (path_var != source_vertex)
        {
            printf("%i -> ", path_var);
            path_var = table[path_var].pre_vertex;
        }
        printf("%i\n", path_var);
    }
    printf("\n");


    // free the graph and the array of linked lists
    free_graph(graph);

    // free the table used to store output of algorithm
    free(table);

    fclose(graph_file);

    return 0;
}
