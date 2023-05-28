#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* represents a vertex or node adjacent to the current vertex in the array
stored inside of a linked list */
typedef struct vertex
{
    int n; // integer name
    int w; // weight of edge
    struct vertex* next;
}vertex;

/* Represents the entire graph. Vertices are extracted from the graph file.
Also stores an array of pointers - array of linked lists.
The linked lists contain vertex types. Example: linked_list_arr[1] is a list
of vertices adjacent (connected by an edge) to vertex 1 on the graph.*/
typedef struct graph_t
{
    int vertices; // number of vertices in the graph
    /* linked_list_arr is an array of pointers to vertex objects */
    vertex** linked_list_arr;
}graph_t;

// Used to store the data extracted from file for the creation of an edge
typedef struct edge_data
{
    int v1, v2, weight;
}edge_data;

/*
Max vertices = 100 (0 - 99)
Max edges = 1000
Max weight = 1 - 1000
Example: 99,1000 longest first line
Example for an edge line max: 99,99,1000 - edge between 99 and 98 weight 1000

Giving non-positive integers from outside the ranges stated will cause the program to behave
unepectedly and likely crash.
*/

#define MAX_LINE_ONE 7 // first line in file cannot exceed 7 characters
#define MAX_OTHER_LINE 10 // Lines 1 - N cannot exceed 10 characters


// function prototypes
vertex* create_vertex(int name, int weight);
void add_edge(graph_t* input_graph, int v1, int v2, int weight);
int search_graph(graph_t* input_graph, int v1, int v2);
void print_graph(graph_t* input_graph);
int read_graph_vertices(FILE* infile);
int read_edge(FILE* infile, edge_data* edge);
void free_graph(graph_t* input_graph);


/* create the graph */
graph_t* create_graph(int number_of_vertices)
{
    // allocate memory to the graph type
    graph_t* new_graph = malloc(sizeof(graph_t));
    if (new_graph == NULL)
    {
        return new_graph;
    }

    /* allocate memory to an array
    Array indexes : vertex names
    Array contains pointers to the head of linked-lists */
    vertex** parray = malloc(sizeof(vertex*) * number_of_vertices);
    if (parray == NULL)
    {
        return new_graph;
    }
    // initialise array elements to point null
    for (int i = 0; i < number_of_vertices; i++)
    {
        parray[i] = NULL;
    }

    new_graph->linked_list_arr = parray; // pointer to array of pointers (type **), stored inside graph
    new_graph->vertices = number_of_vertices;

    return new_graph;
}

// create a vertex (or node) to insert into linked list
vertex* create_vertex(int name, int weight)
{
    vertex* new_vertex = malloc(sizeof(vertex));
    if (new_vertex == NULL)
    {
        printf("error\n");
        exit(1);
    }
    new_vertex->n = name;
    new_vertex->w = weight;
    new_vertex->next = NULL;

    return new_vertex;
}

// check if there is an edge between two vertices in graph
int search_graph(graph_t* input_graph, int v1, int v2)
{
    for (vertex* tmp = input_graph->linked_list_arr[v1]; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->n == v2)
        {
            return 1; // true, v2 already in adj list of v1
        }
    }
    for (vertex* tmp = input_graph->linked_list_arr[v2]; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->n == v1)
        {
            return 1; // true, v1 already in adj list of v2
        }
    }
    return 0; // no edge exists in graph between v1&v2
}


/* Between two vertices create an edge, if one doesn't
already exist. Return errors if vertices out of range */
void add_edge(graph_t* input_graph, int v1, int v2, int weight)
{
    int vertices = input_graph->vertices;
    // check vertices in range and not equal
    if ((v1 < 0) || (v1 >= vertices) || (v2 < 0) || (v2 >= vertices))
    {
        printf("error: One of more vertex values out of range.\n");
        exit(1);
    }
    if (v1 == v2)
    {
        printf("error: At least one looping edge attempted creation.\n");
        exit(1);
    }
    if (weight <= 0)
    {
        printf("error: Edge weight less than or equal to zero attempted creation.\n");
        exit(1);
    }

    // create new vertices
    vertex* new_vertex1 = create_vertex(v1, weight);
    vertex* new_vertex2 = create_vertex(v2, weight);

    // check if edge already exists in graph
    if (search_graph(input_graph, v1, v2))
    {
        printf("error: attempted creation of duplicated edge\n");
        exit(1);
    }

    /* insert vertex to top of list, point next to curr head vertex
       Then vertex in graph to new vertex (now head of linked list) */
    new_vertex1->next = input_graph->linked_list_arr[v2];
    input_graph->linked_list_arr[v2] = new_vertex1;
    new_vertex2->next = input_graph->linked_list_arr[v1];
    input_graph->linked_list_arr[v1] = new_vertex2;

    return;
}

// prints the graph neatly to stdout
void print_graph(graph_t* input_graph)
{
    printf("\n");
    for (int i = 0; i < input_graph->vertices; i++)
    {
        printf("%i : ", i);
        for (vertex* tmp = input_graph->linked_list_arr[i]; tmp != NULL; tmp = tmp->next) // tmp == NULL is final element of linked list
        {
            printf("%i (%i) -> ", tmp->n, tmp->w);
        }
        printf("NULL\n");
    }
    printf("\n");
}

/* iterate over all elements of the adj array and free
each linked list they point to */
void free_graph(graph_t* input_graph)
{
    for (int i = 0; i < input_graph->vertices; i++)
    {
        // recursive function to free linked list
        while (input_graph->linked_list_arr[i] != NULL)
        {
            vertex* tmp = input_graph->linked_list_arr[i]->next;
            free(input_graph->linked_list_arr[i]);
            input_graph->linked_list_arr[i] = tmp;
        }
    }
    // print_graph(input_graph);

    free(input_graph->linked_list_arr);
    free(input_graph);
}

/* read the first line of the file and extract the vertices to create
 the adjacency list array */
int read_graph_vertices(FILE* infile)
{
    char buffer[MAX_LINE_ONE + 1];
    fgets(buffer, MAX_LINE_ONE, infile);

    // parse the buffer and extract actual file contents
    int count = 0;
    for (int i = 0; i < MAX_LINE_ONE + 1; i++)
    {
        if ( ((buffer[i] > 47) && (buffer[i] < 58)) || buffer[i] == 44)
        {
            count++;
        }
        else
        {
            break;
        }
    }

    // move results to new resized buffer on the heap
    char *new_buffer = malloc(sizeof(char) * count + 1);
    if (new_buffer == NULL)
    {
        // program failed return false
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        new_buffer[i] = buffer[i];
    }

    // extract the vertices
    char *token = strtok(new_buffer, ",");

    // convert to type int
    int vertices = atoi(token);

    free(new_buffer);

    return vertices;
}


/* reads a next line from the graph file and updates
variables at v1 and v2 and weight, to use in add_edge */
int read_edge(FILE* infile, edge_data* edge)
{
    char buffer[MAX_OTHER_LINE];
    if (fgets(buffer, MAX_OTHER_LINE, infile) == NULL)
    {
        // reached EOF or error, return false
        return 0;
    }

    // Convert the buffer string to an array of 3 ints
    int line[3];
    char* portion;
    for (int i = 0; i < 3; i++)
    {
        if (i==0)
        {
            portion = strtok(buffer, ",");
        }
        else
        {
            portion = strtok(NULL, ",");
        }
        line[i] = atoi(portion);
    }

    edge->v1 = line[0];
    edge->v2 = line[1];
    edge->weight = line[2];

    return 1;
}