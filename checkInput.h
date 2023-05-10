#ifndef CHECKINPUT_H
#define CHECKINPUT_H

#include "visualization.h"

int input_node_id(const char *prompt, Node *nodes, int num_nodes);

int input_link_id(const char *prompt, Link *links, int num_links);

double input_speed(const char *prompt);

double input_coordinate(const char *prompt, double min_value, double max_value);

#endif