#include "graph.h"

// Display menu options //
void prompt(void)
{
    printf("\nMenu:\n");
    printf("1. Display Adjacency List\n");
    printf("2. Perform Breadth-First Search (BFS)\n");
    printf("3. Perform Depth-First Search (DFS)\n");
    printf("4. Find Shortest Path using Dijkstra's Algorithm\n");
    printf("5. Exit\n");
}

// Reads a graph from a file and constructs the adjacency matrix // 
Graph *readGraph(const char *filename) {
    // Open file
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) {
        fclose(file);
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    graph->numVertices = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjList[i] = NULL;
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }

    // Read matrix data from file
    char line[1024];
    int size = 0;
    rewind(file);
    
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < MAX_VERTICES) {
        char *token = strtok(line, " \t\n");
        int j = 0;
        
        // Parse each line into adjacency matrix
        while (token && j < MAX_VERTICES) {
            graph->adjMatrix[i][j] = atoi(token);
            token = strtok(NULL, " \t\n");
            j++;
        }
        
        // Verify matrix dimensions
        if (i == 0) {
            size = j;
        } else if (j != size) {
            fprintf(stderr, "Error: Inconsistent matrix dimensions\n");
            free(graph);
            fclose(file);
            return NULL;
        }
        i++;
    }

    // Verify square matrix
    if (i != size) {
        fprintf(stderr, "Error: Matrix is not square\n");
        free(graph);
        fclose(file);
        return NULL;
    }

    graph->numVertices = size;
    fclose(file);
    
    createAdjacencyList(graph);
    return graph;
}

// Creates a new node for the adjacency list
Node *createNode(int vertex, int weight) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Creates the adjacency list from the adjacency matrix
void createAdjacencyList(Graph *graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        graph->adjList[i] = NULL;
        Node *last = NULL;
        
        for (int j = 0; j < graph->numVertices; j++) {
            if (graph->adjMatrix[i][j] != 0) {
                Node *newNode = createNode(j, graph->adjMatrix[i][j]);
                if (!newNode) continue;
                
                // Add node to list
                if (!graph->adjList[i]) {
                    graph->adjList[i] = newNode;
                } else {
                    last->next = newNode;
                }
                last = newNode;
            }
        }
    }
}

// Displays the adjacency list
void displayAdjacencyList(Graph *graph) 
{
    printf("\nAdjacency List:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %d:", i + 1);
        Node *temp = graph->adjList[i];
        while (temp) {
            printf(" -> %d(%d)", temp->vertex + 1, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

void bfs(Graph *graph, int startVertex) {
    // Allocate and initialize visited array
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    // Allocate queue for BFS
    int *queue = (int *)malloc(graph->numVertices * sizeof(int));
    if (!queue) {
        free(visited);
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    int front = 0, rear = 0;
    printf("\nBFS traversal starting from vertex %d:", startVertex + 1);
    
    // BFS algorithm implementation
    visited[startVertex] = true;
    queue[rear++] = startVertex;

    while (front < rear) {
        int currentVertex = queue[front++];
        printf(" %d", currentVertex + 1);

        Node *temp = graph->adjList[currentVertex];
        while (temp) {
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = true;
                queue[rear++] = temp->vertex;
            }
            temp = temp->next;
        }
    }
    printf("\n");

    free(visited);
    free(queue);
}

// DFS implementation
void dfs(Graph *graph, int startVertex) 
{
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited) 
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    printf("\nDFS traversal starting from vertex %d:", startVertex + 1);
    dfsRecursive(graph, startVertex, visited);
    printf("\n");
    
    free(visited);
}

void dfsRecursive(Graph *graph, int vertex, bool *visited)
{
    visited[vertex] = true;
    printf(" %d", vertex + 1);

    Node *temp = graph->adjList[vertex];
    while (temp)
    {
        if (!visited[temp->vertex])
        {
            dfsRecursive(graph, temp->vertex, visited);
        }
        temp = temp->next;
    }
}

// Finds the shortest path from the start vertex to all other vertices using Dijkstra's algorithm
void dijkstra(Graph *graph, int startVertex) {
    // Allocate arrays for distances and visited vertices
    int *dist = (int *)malloc(graph->numVertices * sizeof(int));
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    
    if (!dist || !visited) {
        free(dist);
        free(visited);
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }

    // Initialize distances
    for (int i = 0; i < graph->numVertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[startVertex] = 0;

    // Dijkstra's algorithm implementation
    for (int count = 0; count < graph->numVertices - 1; count++) {
        int minDist = INT_MAX, minVertex = -1;
        
        // Find vertex with minimum distance
        for (int v = 0; v < graph->numVertices; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                minVertex = v;
            }
        }

        if (minVertex == -1) break;
        
        visited[minVertex] = true;

        // Update distances to adjacent vertices
        Node *temp = graph->adjList[minVertex];
        while (temp) {
            int v = temp->vertex;
            if (!visited[v] && dist[minVertex] != INT_MAX && 
                dist[minVertex] + temp->weight < dist[v]) {
                dist[v] = dist[minVertex] + temp->weight;
            }
            temp = temp->next;
        }
    }

    // Print results
    printf("\nShortest paths from vertex %d:\n", startVertex + 1);
    for (int i = 0; i < graph->numVertices; i++) {
        if (i != startVertex) {
            if (dist[i] == INT_MAX)
                printf("No path to vertex %d\n", i + 1);
            else
                printf("Distance to vertex %d: %d\n", i + 1, dist[i]);
        }
    }

    free(dist);
    free(visited);
}

// Frees the memory allocated for the graph aswell as the adjacency list nodes
void freeGraph(Graph *graph) 
{
    if (!graph) return;
        for (int i = 0; i < graph->numVertices; i++) 
        {
        Node *current = graph->adjList[i];
        while (current) 
        {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}