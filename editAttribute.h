#ifndef EDIT_ATTRIBUTES_H
#define EDIT_ATTRIBUTES_H

#include "storeData.h"

int compare_links(const void *a, const void *b);

int compare_nodes(const void *a, const void *b);

void save_data(const char *original_filename, const char *parsed_filename, Node *nodes, int num_nodes, Link *links, int num_links);

void update_link_attribute(const char *input_file, Node *nodes, DataLists data_lists, Link *links, int link_count, int link_id, const char *attribute, double new_value);

void update_node_attribute(const char *input_file, Node *nodes, DataLists data_lists, Link *links, int node_count, int node_id, const char *attribute, double new_value);

#endif