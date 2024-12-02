#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100

// Structure for adjacency list node
typedef struct Node 
{
    int vertex;
    int weight;
    struct Node* next;
} Node;

// Structure for graph
typedef struct Graph 
{
    int numVertices;
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
    Node* adjList[MAX_VERTICES];
} Graph;

// Function prototypes
void prompt(void);
Graph* readGraph(const char* filename);
Node* createNode(int vertex, int weight);
void createAdjacencyList(Graph* graph);
void displayAdjacencyList(Graph* graph);
void bfs(Graph* graph, int startVertex);
void dfs(Graph* graph, int startVertex);
void dfsRecursive(Graph* graph, int vertex, bool* visited);
void dijkstra(Graph* graph, int startVertex);
void freeGraph(Graph* graph);

#endif