#ifndef FIND_ROUTE_H
#define FIND_ROUTE_H

#include <math.h>

#include "visualization.h"

void free_path(Path *path);

double calculate_length(Link link);

double calculate_time(Link link);

int find_min_weight(double *distances, bool *visited, int num_nodes);

Path dijkstra_algorithm(Node *nodes, int num_nodes, Link *links, int num_links, int start_id, int end_id, double (*weight_function)(Link));

double path_length(Path path, Link *links, int num_links);

Path constrained_shortest_path(Node *nodes, int num_nodes, Link *links, int num_links, int start_id, int end_id, int *intermediate_nodes, int num_intermediate_nodes);

#endif