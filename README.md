
# Dijkstra's algorithm implementation for Undirected Weighted Graphs
#### Video Demo: <https://youtu.be/xSOCNimSzi0>
#### Description:
Implementation of Djikstra's Shortest Path Algorithm for undirected, weighted graphs.
The program reads an input graph from pre-defined formatted `.txt` file. This file contains the nodes of the graph and the edge weights between them. The program is ran by calling main and specifying the input graph filename.

`$ ./main <input_graph.txt> <source vertex>`


In addition the source vertex is specified. It can be any node (numbered 0 - N) in the graph.

Maxium amount of vertices (or nodes) is 100, [named 0-99]. The maxium amount of edges is 1000. The maximum weight an edge can be assigned is 1000. There is not support for looping edges or nodes that have no edges (isolated nodes). These aspects where the program can be improved upon.

The program reads from the supplied text file and loads the graph data structure into memory. This implementation uses adjacency lists, whereby each linked list is pointed to from an array (making up the graph). Alternative structures like an adjacency matrix could have been used. The advantage given by the adjacency list is that it is memory efficient when representing sparse graphs.

The program then builds a results table data structure - storing parent nodes, distance from source node, and visited boolean values for each node.
The algorithm runs and prints a representation of the graph data structure to the terminal and then prints the shortest distance from each node to the source node, followed by the path to get there.
Some example graphs are included in the source code. The following is an example of the execution:

```
project/ $ ./main "example_graphs/graph2.txt" 0

0 : 2 (4) -> 1 (1) -> NULL
1 : 4 (7) -> 3 (2) -> 2 (4) -> 0 (1) -> NULL
2 : 4 (5) -> 1 (4) -> 0 (4) -> NULL
3 : 5 (6) -> 4 (4) -> 1 (2) -> NULL
4 : 5 (7) -> 3 (4) -> 2 (5) -> 1 (7) -> NULL
5 : 4 (7) -> 3 (6) -> NULL

Vertex 0: 0
0
Vertex 1: 1
1 -> 0
Vertex 2: 4
2 -> 0
Vertex 3: 3
3 -> 1 -> 0
Vertex 4: 7
4 -> 3 -> 1 -> 0
Vertex 5: 9
5 -> 3 -> 1 -> 0
```

To save a new graph as a text file, the following format must be followed.

first line - number of vertices,number of edges
e.g. `6,10` for a graph with `6` nodes and `10` edges
There after each line defines an edge:
`0,1,2` - defines an edge of weight `2`, between node `0` and `1`.
Duplicated edges are ignored by the code. Edges cannot be loops (e.g. `1,1,1` not allowed).
Edge weight must be greater than 0 and less than 1000.

An example graph could be defined as `graph.txt`:
```
6,10
0,1,1
0,2,4
1,2,4
1,3,2
1,4,7
2,4,5
3,4,4
3,5,6
4,5,7
```