#ifndef EDIT_ATTRIBUTES_H
#define EDIT_ATTRIBUTES_H

#include "readMap.h"

#define MAX_POI_COUNT 10

void save_data(const char *original_filename, Node *nodes, Link *links, DataLists *data_lists);

void update_link_attribute(const char *input_file, Node *nodes, Link *links, DataLists* data_lists, int link_id, const char *attribute, double new_value);

void update_node_attribute(const char *input_file, Node *nodes, Link *links, DataLists* data_lists, int node_id, const char *attribute, double new_value);

void update_poi_attribute(const char *input_file, Node *nodes, Link *links, DataLists* data_lists, int link_id, int new_poi_id);

#endif