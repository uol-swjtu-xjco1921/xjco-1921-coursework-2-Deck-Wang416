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
double calculate_length(Link link)
{
    return link.length;
}

// Calculate the time to pass through a road
double calculate_time(Link link)
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

Path dijkstra_algorithm(Node *nodes, int num_nodes, Link *links, int num_links, int start_index, int end_index, double (*weight_function)(Link))
{
    double *weights = (double *)malloc(num_nodes * sizeof(double));

    int *previous_nodes = (int *)malloc(num_nodes * sizeof(int));

    bool *visited = (bool *)malloc(num_nodes * sizeof(bool));

    Path result = {0, NULL, 0};

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
                double new_weight = weights[current_node_index] + weight_function(links[j]);

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
        printf("No Appropriate Path Found.\n");

        result.length = EXIT_WITH_ERRORS;

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

double path_length(Path path, Link *links, int num_links) 
{
    double total_length = 0;

    for (int i = 0; i < path.length - 1; i++) 
    {
        int current_node_id = path.path[i].id;

        int next_node_id = path.path[i + 1].id;
        
        for (int j = 0; j < num_links; j++) 
        {
            if ((links[j].node1 == current_node_id && links[j].node2 == next_node_id) ||
                (links[j].node2 == current_node_id && links[j].node1 == next_node_id)) 
                {
                    total_length += links[j].length;

                    break;
                }
        }
    }

    return total_length;
}

Path shortest_path_with_positions(Node *nodes, int num_nodes, Link *links, int num_links, int start_id, int end_id, int *intermediate_nodes, int num_intermediate_nodes) 
{
    Path constrained_path = {0, NULL, 0};

    int previous_node_id = start_id;

    double total_length = 0;

    for (int i = 0; i <= num_intermediate_nodes; i++)
    {
        int next_node_id = (i == num_intermediate_nodes) ? end_id : intermediate_nodes[i];

        int start_index = find_node_index(nodes, num_nodes, previous_node_id);

        int end_index = find_node_index(nodes, num_nodes, next_node_id);

        Path sub_path = dijkstra_algorithm(nodes, num_nodes, links, num_links, start_index, end_index, calculate_length);

        if (sub_path.length == EXIT_WITH_ERRORS) 
        {
            printf("No Constrained Path Found.\n");

            free_path(&constrained_path);

            return constrained_path;
        }

        // Concatenate the sub_path to the constrained_path
        constrained_path.path = realloc(constrained_path.path, (constrained_path.length + sub_path.length - 1) * sizeof(Node));

        memcpy(constrained_path.path + constrained_path.length, sub_path.path + 1, (sub_path.length - 1) * sizeof(Node));

        constrained_path.length += sub_path.length - 1;

        total_length += path_length(sub_path, links, num_links);

        previous_node_id = next_node_id;

        free_path(&sub_path);
    }

    constrained_path.total_length = total_length;

    return constrained_path;
}

bool is_poi_on_link(Link *link, int poi_id) 
{
    // Iterate over the POIs on the link
    for (int i = 0; i < link->poi_count; i++) 
    {
        if (link->POI[i] == poi_id) 
        {
            return true;
        }
    }

    return false;
}

Link* find_link_between_nodes(Link* links, int num_links, int node1_id, int node2_id) 
{
    for (int i = 0; i < num_links; i++) 
    {
        if ((links[i].node1 == node1_id && links[i].node2 == node2_id) ||
            (links[i].node1 == node2_id && links[i].node2 == node1_id))
            {
                return &links[i];
            }
    }

    return NULL;
}

Path shortest_path_with_pois(Node *nodes, int num_nodes, Link *links, int num_links, int start_id, int end_id, int *intermediate_poi_ids, int num_intermediate_pois) 
{
    Path shortest_path = {0, NULL, INFINITY};

    // Check if start node ID or end node ID are in links with POI
    for (int i = 0; i < num_intermediate_pois; i++) 
    {
        if (find_link_between_nodes(links, num_links, start_id, end_id) == NULL)
            {
                break;
            }

        if (is_poi_on_link(find_link_between_nodes(links, num_links, start_id, end_id), intermediate_poi_ids[i]) == true)
            {
                // If start_id or end_id is a POI, construct a direct path
                int start_index = find_node_index(nodes, num_nodes, start_id);

                int end_index = find_node_index(nodes, num_nodes, end_id);

                shortest_path = dijkstra_algorithm(nodes, num_nodes, links, num_links, start_index, end_index, calculate_length);
      
                return shortest_path;
            }
    }

    // For each intermediate POI
    for (int i = 0; i < num_intermediate_pois; i++) 
    {
        // Find all links containing the current POI
        Link* links_with_poi[MAX_LINKS]; 

        int num_links_with_poi = 0;

        for (int j = 0; j < num_links; j++) 
        {
            if (is_poi_on_link(&links[j], intermediate_poi_ids[i])) 
            {
                links_with_poi[num_links_with_poi++] = &links[j];
            }
        }

        // For each link containing the current POI
        for (int j = 0; j < num_links_with_poi; j++) 
        {
            // Compute a path passing through the current link
            int intermediate_nodes[2] = {links_with_poi[j]->node1, links_with_poi[j]->node2};

            Path current_path = shortest_path_with_positions(nodes, num_nodes, links, num_links, start_id, end_id, intermediate_nodes, 2);

            // If current path is shorter than the shortest path found so far, update the shortest path
            if (current_path.total_length < shortest_path.total_length) 
            {
                free_path(&shortest_path);

                shortest_path = current_path;
            } 

            else 
            {
                free_path(&current_path);
            }
        }
    }

    if (shortest_path.length == 0) 
    {
        printf("No Constrained Path Found.\n");
    }

    return shortest_path;
}