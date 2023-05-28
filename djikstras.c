#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


/* Implementation of Djikstra's Shortest Path Algorithm for undirected, weighted graphs.
Maxium amount of vertices (or nodes) is 100, [0-99]. The maxium amount of edges is 1000.
The maximum weight an edge can be assigned is 1000. There is not support for looping edges
(where vertex 1 connects to vertex 1 with weight X). */

// stored inside of table array, vertex name is index
typedef struct table_entry
{
    int dis; // gets initialised to infinity
    int pre_vertex; // parent to each vertex
    bool visited; // mark off as true when visited
}table_entry;

table_entry* create_table(int number_of_vertices, int source_vertex);
void djikstra(table_entry* table, graph_t* graph);
int choose_next_vertex(table_entry* table, graph_t*);
int check_visited(table_entry* table, graph_t* graph);


// reminder - look at how to initialise a pointer to an array in a struct and with normal variables
// get to the bottom of is ** pointer to an array or pointer to array of pointers?
void djikstra(table_entry* table, graph_t* graph)
{
    while (!check_visited(table, graph)) // repeat until all vertices are visited
    {
        /* Choose the next vertex. Must be unvisited vertex and have lowest distance */
        int curr_vertex = choose_next_vertex(table, graph);

        table[curr_vertex].visited = true; // mark vertex as visited

        /* iterate through adj_list of curr_vertex in the graph */
        for (vertex* tmp = graph->linked_list_arr[curr_vertex]; tmp != NULL; tmp = tmp->next)
        {
            // relax edges, calc distances for unvisited vertices
            if (table[tmp->n].visited == false)
            {
                int new_dis = (table[curr_vertex].dis) + (tmp->w); // tmp->w is edge weight to adj vertex
                // if new_dis is lower, update table
                if (new_dis < table[tmp->n].dis) {
                    table[tmp->n].dis = new_dis;
                    table[tmp->n].pre_vertex = curr_vertex;
                }
            }
        }
    }
    return;
}


int check_visited(table_entry* table, graph_t* graph)
{
    /* check to see if all (vertices - 1) are visited, return true if they are all visited */

    // count trues in visited array of table
    int count = 0;
    for (int i = 0; i < graph->vertices; i++)
    {
        if (table[i].visited == true)
        {
            count++;
        }
    }

    // compare count to number of vertices to decide if all are visited
    if (count == (graph->vertices - 1)) {
        return true;
    }
    else {
        return false;
    }
}


int choose_next_vertex(table_entry* table, graph_t* graph)
{
    /* find vertex with smallest dis that is unvisited */

    // there will always be a distance smaller than int_max (source is assigned)
    int smallest = INT_MAX; // for comparing array vals too
    int index; // to store the vertex of the smallest
    for (int i = 0; i < graph->vertices; i++)
    {
        if ((table[i].dis < smallest) && (table[i].visited == false))
        {
            smallest = table[i].dis;
            index = i;
        }
    }
    return index;
}


table_entry* create_table(int number_of_vertices, int source_vertex)
{
    // array of structs
    table_entry* table = malloc(sizeof(table_entry) * number_of_vertices);
    if (table == NULL)
    {
        return table;
    }

    for (int i = 0; i < number_of_vertices; i++)
    {
        // allocated on the stack
        table_entry row;

        // mark the source distance as 0
        if (i == source_vertex)
        {
            row.dis = 0;
        }
        else
        {
            row.dis = INT_MAX;
        }
        // row.dis = INT_MAX;
        row.pre_vertex = -1;
        row.visited = false;
        table[i] = row;
    }

    return table;
}


// rename the graph type to graph_t?