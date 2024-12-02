#include "graph.h"

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    Graph *graph = readGraph(argv[1]);
    if (!graph) 
    {
        fprintf(stderr, "Failed to create graph from file\n");
        return 1;
    }

    int choice;
    int startVertex;

    // MAIN LOOP //
    do
    {
        prompt();
        printf("Enter your choice: ");

        // Case for invalid choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice)
        {
        case 1:
            displayAdjacencyList(graph);
            break;

        case 2:
            printf("Enter starting vertex (1 to %d): ", graph->numVertices);
            if (scanf("%d", &startVertex) != 1 || 
                startVertex < 1 || 
                startVertex > graph->numVertices) {
                printf("Invalid vertex number.\n");
                while (getchar() != '\n'); // Clear input buffer
                break;
            }
            bfs(graph, startVertex - 1); // Convert to 0-based indexing
            break;

        case 3:
            printf("Enter starting vertex (1 to %d): ", graph->numVertices);
            if (scanf("%d", &startVertex) != 1 || 
                startVertex < 1 || 
                startVertex > graph->numVertices) {
                printf("Invalid vertex number.\n");
                while (getchar() != '\n'); // Clear input buffer
                break;
            }
            dfs(graph, startVertex - 1); // Convert to 0-based indexing
            break;

        case 4:
            printf("Enter starting vertex (1 to %d): ", graph->numVertices);
            if (scanf("%d", &startVertex) != 1 || 
                startVertex < 1 || 
                startVertex > graph->numVertices) {
                printf("Invalid vertex number.\n");
                while (getchar() != '\n'); // Clear input buffer
                break;
            }
            dijkstra(graph, startVertex - 1); // Convert to 0-based indexing
            break;

        case 5:
            printf("Exiting...\n");
            freeGraph(graph);
            return 0;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

