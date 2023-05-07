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
double calculate_length(Node node1, Node node2, Link link)
{
    // Pythagorean Theorem
    return hypot(node2.lon - node1.lon, node2.lat - node1.lat);
}

// Calculate the time to pass through a road
double calculate_time(Node node1, Node node2, Link link)
{
    double time = link.length / link.speed;

    return time;
}

// Find the node with the minimum weight
int find_min_weight(double *weights, bool *visited, int num_nodes) 
{
    // Initialize to the maximum value of type double (infinitely far)
    double min_weight = DBL_MAX;

    // Index of nodes with minimum weight
    int min_index = -1;

    for (int i = 0; i < num_nodes; i++) 
    {
        // When the node has not been accessed, and the weight is less than current minimum weight:
        if (!visited[i] && weights[i] < min_weight) 
        {
            min_weight = weights[i];

            min_index = i;
        }
    }
    
    return min_index;
}

Path dijkstra_algorithm(Node *nodes, int num_nodes, Link *links, int num_links, int start_index, int end_index, double (*weight_function)(Node, Node, Link)) 
{
    double *weights = (double *)malloc(num_nodes * sizeof(double));

    int *previous_nodes = (int *)malloc(num_nodes * sizeof(int));

    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));

    Path result = {0, NULL};

    // Allocate an initial size of memory
    result.path = (Node *)calloc(1, sizeof(Node));
 
    for (int i = 0; i < num_nodes; i++) 
    {
        weights[i] = DBL_MAX;
 
        visited[i] = false;
    }

    weights[start_index] = 0;

    for (int i = 0; i < num_nodes - 1; i++) 
    {
        int current_node_index = find_min_weight(weights, visited, num_nodes);

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
                // Calculate new weight from current node to neighboring nodes
                double new_weight = weights[current_node_index] + weight_function(nodes[current_node_index], nodes[neighbor_index], links[j]);

                // If new weight is less than the previous one, update weight and set current node as previous node
                if (new_weight < weights[neighbor_index]) 
                {
                    weights[neighbor_index] = new_weight;

                    previous_nodes[neighbor_index] = current_node_index;
                }
            }
        }
    }

    if (weights[end_index] == DBL_MAX) 
    {
        printf("No Path Found between Nodes %d And %d.\n", start_index, end_index);

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
    free(weights);

    free(previous_nodes);

    free(visited);

    printf("FOUND\n");

    return result;
}