#include "editAttribute.h"

// Save the updated data to original and parsed files
void save_data(const char *original_filename, Node *nodes, Link *links, DataLists *data_lists)
{
    // Save to original file
    FILE *original_file = fopen(original_filename, "w");

    if (original_file == NULL)
    {
        printf("ERROR: Bad File Name (%s)\n", original_filename);

        exit(EXIT_BAD_FILE_NAME);
    }

    // Write bounding data to original file
    fprintf(original_file, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>\n", data_lists->bounding.minLat, data_lists->bounding.minLon, data_lists->bounding.maxLat, data_lists->bounding.maxLon);

    // Write links data to original file
    for (int i = data_lists->link_count - 1; i >= 0; i--)
    {
        fprintf(original_file, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf POI=", links[i].id, links[i].node1, links[i].node2, links[i].way, links[i].length, links[i].veg, links[i].arch, links[i].land, links[i].speed);

        // Add POIs
        for (int j = 0; j < links[i].poi_count; j++) 
        {
            fprintf(original_file, "%d,", links[i].POI[j]);
        }

        fprintf(original_file, ";/link>\n");
    }

    // Write nodes data to original file
    for (int i = data_lists->node_count - 1; i >= 0; i--)
    {
        fprintf(original_file, "<node id=%d lat=%lf lon=%lf /node>\n", nodes[i].id, nodes[i].lat, nodes[i].lon);
    }

    // Write unparsed tags (way and geom) data to original file
    UnparsedTag *current_unparsed = data_lists->unparsedTags;

    while (current_unparsed != NULL) 
    {
        fprintf(original_file, "%s", current_unparsed->line);

        current_unparsed = current_unparsed->next;
    }

    fclose(original_file);
}

// Update the attribute of specified links
void update_link_attribute(const char *input_file, Node *nodes, Link *links, DataLists* data_lists, int link_id, const char *attribute, double new_value)
{
    // Iterate to find the link with specified ID
    for (int i = 0; i < data_lists->link_count; i++)
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

            // Save the updated data to original file
            save_data(input_file, nodes, links, data_lists);

            break;
        }
    }
}

// Update the attribute of specified nodes
void update_node_attribute(const char *input_file, Node *nodes, Link *links, DataLists* data_lists, int node_id, const char *attribute, double new_value)
{
    // Iterate to find the node with specified ID
    for (int i = 0; i < data_lists->node_count; i++)
    {
        // Update the specified attribute of the node with new value
        if (nodes[i].id == node_id)
        {
            if (strcmp(attribute, "lat") == 0)

                nodes[i].lat = new_value;

            else if (strcmp(attribute, "lon") == 0)

                nodes[i].lon = new_value;

            // Save the updated data to original file
            save_data(input_file, nodes, links, data_lists);

            break;
        }
    }
}

// Update the POIs of specified links
void update_poi_attribute(const char *input_file, Node *nodes, Link *links, DataLists* data_lists, int link_id, int new_poi_id)
{
    // Iterate to find the link with specified ID
    for (int i = 0; i < data_lists->link_count; i++)
    {
        // Add the new POI to the link if it is the specified one
        if (links[i].id == link_id)
        {
            // Check if there is enough space to add a new POI
            if (links[i].poi_count < MAX_POI_COUNT)
            {
                // Add new POI at the end of the POI array
                links[i].POI[links[i].poi_count] = new_poi_id;
                
                // Increase the POI count
                links[i].poi_count++;

                // Save the updated data to original file
                save_data(input_file, nodes, links, data_lists);
            }

            else
            {
                printf("Usage: The Link Already Has The Most POIs (%d).\n", MAX_POI_COUNT);
            }

            break;
        }
    }
}