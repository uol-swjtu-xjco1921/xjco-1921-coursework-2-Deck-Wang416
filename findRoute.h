#ifndef FIND_ROUTE_H
#define FIND_ROUTE_H

#include <math.h>

#include "visualization.h"

void free_path(Path *path);

double calculate_length(Node node1, Node node2, Link link);

double calculate_time(Node node1, Node node2, Link link);

int find_min_weight(double *distances, bool *visited, int num_nodes);

Path dijkstra_algorithm(Node *nodes, int num_nodes, Link *links, int num_links, int start_id, int end_id, double (*weight_function)(Node, Node, Link));

#endif