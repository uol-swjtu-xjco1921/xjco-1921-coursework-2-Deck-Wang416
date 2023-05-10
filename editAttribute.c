#include "editAttribute.h"

// Compare function for “qsort” to sort links by ID
int compare_links(const void *a, const void *b)
{
    Link *link1 = (Link *)a;

    Link *link2 = (Link *)b;

    return link1->id - link2->id;
}

// Compare function for “qsort” to sort nodes by ID
int compare_nodes(const void *a, const void *b)
{
    Node *node1 = (Node *)a;

    Node *node2 = (Node *)b;

    return node1->id - node2->id;
}

// Save the updated data to original and parsed files
void save_data(const char *original_filename, const char *parsed_filename, Node *nodes, int num_nodes, Link *links, int num_links)
{
    // Sort links array in ascending order
    qsort(links, num_links, sizeof(Link), compare_links);

    // Sort nodes array in ascending order
    qsort(nodes, num_nodes, sizeof(Node), compare_nodes);

    // Save to original file
    FILE *original_file = fopen(original_filename, "w");

    if (!original_file)
    {
        perror("Error opening original file for saving data");

        return;
    }

    // Save to parsed file
    FILE *parsed_file = fopen(parsed_filename, "w");

    if (!parsed_file)
    {
        perror("Error opening parsed file for saving data");

        fclose(original_file);

        return;
    }

    // Write links data to both files
    for (int i = 0; i < num_links; i++)
    {
        fprintf(original_file, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf /link>\n", links[i].id, links[i].node1, links[i].node2, links[i].way, links[i].length, links[i].veg, links[i].arch, links[i].land, links[i].speed);

        fprintf(parsed_file, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf /link>\n", links[i].id, links[i].node1, links[i].node2, links[i].way, links[i].length, links[i].veg, links[i].arch, links[i].land, links[i].speed);
    }

    // Write nodes data to both files
    for (int i = 0; i < num_nodes; i++)
    {
        fprintf(original_file, "<node id=%d lat=%lf lon=%lf /node>\n", nodes[i].id, nodes[i].lat, nodes[i].lon);

        fprintf(parsed_file, "<node id=%d lat=%lf lon=%lf /node>\n", nodes[i].id, nodes[i].lat, nodes[i].lon);
    }

    fclose(original_file);

    fclose(parsed_file);
}

// Update the attribute of specified links
void update_link_attribute(const char *input_file, Node *nodes, DataLists data_lists, Link *links, int link_count, 
int link_id, const char *attribute, double new_value)
{
    // Iterate to find the link with specified ID
    for (int i = 0; i < link_count; i++)
    {
        // Update the specified attribute of the link with new value
        if (links[i].id == link_id)
        {
            if (strcmp(attribute, "length") == 0)

                links[i].length = new_value;

            else if (strcmp(attribute, "veg") == 0)

                links[i].veg = new_value;

            else if (strcmp(attribute, "arch") == 0)

                links[i].arch = new_value;

            else if (strcmp(attribute, "land") == 0)

                links[i].land = new_value;

            else if (strcmp(attribute, "speed") == 0)

                links[i].speed = new_value;

            // Save the updated data to original and parsed files
            save_data(input_file, "data.txt", nodes, data_lists.node_count, links, data_lists.link_count);

            break;
        }
    }
}

// Update the attribute of specified nodes
void update_node_attribute(const char *input_file, Node *nodes, DataLists data_lists, Link *links, int node_count, 
int node_id, const char *attribute, double new_value)
{
    // Iterate to find the node with specified ID
    for (int i = 0; i < node_count; i++)
    {
        // Update the specified attribute of the node with new value
        if (nodes[i].id == node_id)
        {
            if (strcmp(attribute, "lat") == 0)

                nodes[i].lat = new_value;

            else if (strcmp(attribute, "lon") == 0)

                nodes[i].lon = new_value;

            // Save the updated data to original and parsed files
            save_data(input_file, "data.txt", nodes, data_lists.node_count, links, data_lists.link_count);

            break;
        }
    }
}