#include "findRoute.h"

// Free the memory allocated for the path
void free_path(Path *path) 
{
    if (path->path) 
    {
        free(path->path);

        path->path = NULL;
    }

    path->length = 0;
}

// Calculate the Linear Distance between two nodes 
double calculate_length(Node node1, Node node2) 
{
    // Pythagorean Theorem
    return hypot(node2.lon - node1.lon, node2.lat - node1.lat);
}

// Find the node with the minimum distance among the unreachable nodes and return its index
int find_min_distance(double *distances, bool *visited, int num_nodes) 
{
    // Initialize to the maximum value of type double (infinitely far)
    double min_distance = DBL_MAX;

    // Index of nodes with minimum distance value in node array
    int min_index = -1;

    for (int i = 0; i < num_nodes; i++) 
    {
        // When the node has not been accessed, and the distance is less than current minimum distance:
        if (!visited[i] && distances[i] < min_distance) 
        {
            min_distance = distances[i];

            min_index = i;
        }
    }
    return min_index;
}

Path dijkstra_algorithm(Node *nodes, int num_nodes, Link *links, int num_links, int start_id, int end_id) 
{
    double *distances = (double *)malloc(num_nodes * sizeof(double));

    int *previous_nodes = (int *)malloc(num_nodes * sizeof(int));

    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));

    Path result={0, NULL};

    // Allocate an initial size of memory
    result.path = (Node *)calloc(1, sizeof(Node));
 
    for (int i = 0; i < num_nodes; i++) 
    {
        distances[i] = DBL_MAX;
 
        visited[i] = false;
    }

    // Find the index of the starting and ending codes
    int start_index = find_node_index(nodes, num_nodes, start_id);

    int end_index = find_node_index(nodes, num_nodes, end_id);

    if (start_index == -1) 
    {
        printf("ERROR: Bad Start Node ID.\n");

        result.length = -1;

        return result;
    }

    if(end_index == -1)
    {
        printf("ERROR: Bad End Node ID.\n");

        result.length = -1;

        return result;
    }

    distances[start_index] = 0;

    for (int i = 0; i < num_nodes - 1; i++) 
    {
        int current_node_index = find_min_distance(distances, visited, num_nodes);

        if (current_node_index == -1) 
        {
            break;
        }

        visited[current_node_index] = true;

        // Check the neighboring nodes of the current node
        for (int j = 0; j < num_links; j++) 
        {
            int neighbor_index = -1;

            if (links[j].node1 == nodes[current_node_index].id) 
            {
                neighbor_index = find_node_index(nodes, num_nodes, links[j].node2);
            } 
            
            else if (links[j].node2 == nodes[current_node_index].id) 
            {
                neighbor_index = find_node_index(nodes, num_nodes, links[j].node1);
            }

            // If a neighboring node is found and has not yet been accessed:
            if (neighbor_index != -1 && !visited[neighbor_index]) 
            {
                // Calculate new distance from current node to neighboring nodes
                double new_distance = distances[current_node_index] + calculate_length(nodes[current_node_index], nodes[neighbor_index]);

                // If new distance is less than the previous one, update distance and set current node as previous node.
                if (new_distance < distances[neighbor_index]) 
                {
                    distances[neighbor_index] = new_distance;

                    previous_nodes[neighbor_index] = current_node_index;
                }
            }
        }
    }

    if (distances[end_index] == DBL_MAX) 
    {
        printf("ERROR: No Path.\n");

        result.length = -1;

        return result;
    }

    // Backtrack from the endpoint
    int backtrack_node_index = end_index;

    result.length = 0;

    // Check if it has been traced back to the starting point
    while (backtrack_node_index != start_index) 
    {
        result.length++;

        result.path = realloc(result.path, result.length * sizeof(Node));

        result.path[result.length - 1] = nodes[backtrack_node_index];

        backtrack_node_index = previous_nodes[backtrack_node_index];
    }

    // Add the starting point to the array as well
    result.length++;

    result.path = realloc(result.path, result.length * sizeof(Node));

    result.path[result.length - 1] = nodes[start_index];

    // Reverse the path to start from the starting point
    for (int i = 0; i < result.length / 2; i++) 
    {
        Node temp = result.path[i];

        result.path[i] = result.path[result.length - i - 1];

        result.path[result.length - i - 1] = temp;
    }

    // Free allocated memory
    free(distances);

    free(previous_nodes);

    free(visited);

    printf("FOUND\n");

    return result;
}