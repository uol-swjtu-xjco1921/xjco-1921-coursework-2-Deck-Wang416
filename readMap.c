#include "readMap.h"

bool is_bounding_format_correct(const char* line)
{
    double minLat, minLon, maxLat, maxLon;

    int num_parsed = sscanf(line, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>", &minLat, &minLon, &maxLat, &maxLon);

    return num_parsed == 4;
}

bool is_link_format_correct(const char* line)
{
    int id, node1, node2, way;

    double length;

    int num_parsed = sscanf(line, "<link id=%d node=%d node=%d way=%d length=%lf", &id, &node1, &node2, &way, &length);

    return num_parsed == 5;
}

bool is_node_format_correct(const char* line)
{
    int id;

    double lat,lon;

    int num_parsed = sscanf(line, "<node id=%d lat=%lf lon=%lf", &id, &lat, &lon);

    return num_parsed == 3;
}

// Free the memory allocated for the list
void free_list(void *list, ListType type) 
{
    switch (type) 
    {
        case LINK_LIST: 
        {
            LinkList *linkTemp;

            LinkList *linkList = (LinkList *)list;

            // Iterate through the linkList and free each element
            while (linkList) 
            {
                linkTemp = linkList;

                linkList = linkList->next;

                free(linkTemp);
            }

            break;
        }
        
        case NODE_LIST: 
        {
            NodeList *nodeTemp;

            NodeList *nodeList = (NodeList *)list;

            // Iterate through the nodeList and free each element
            while (nodeList) 
            {
                nodeTemp = nodeList;

                nodeList = nodeList->next;

                free(nodeTemp);
            }

            break;
        }

        default:
        
            break;
    }
}

void free_unparsed_tags(UnparsedTag *list) 
{
    UnparsedTag *current = list;

    while (current != NULL) 
    {
        UnparsedTag *next = current->next;

        free(current->line);

        free(current);

        current = next;
    }
}

DataLists parse_and_store_data(const char *filename)
{
    DataLists data_lists = {NULL, NULL, {0, 0, 0, 0}, 0, 0};

    FILE *file = fopen(filename, "r");

    // Error handling for file opening
    if (file == NULL)
    {
        printf("ERROR: Bad File Name (%s)\n", filename);

        exit(EXIT_BAD_FILE_NAME);
    }

    char line[MAX_LINE_LENGTH];

    bool foundBounding = false;

    bool foundLink = false;

    bool foundNode = false;

    // Read the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        if (strncmp(line, "<bounding", 9) == 0)
        {
            // Determine if key labels are missed
            if (!is_bounding_format_correct(line))
            {
                printf("ERROR: Bad Attributes (%s)\n", filename);

                exit(EXIT_BAD_ATTRIBUTES);
            }

            sscanf(line, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>", &data_lists.bounding.minLat, &data_lists.bounding.minLon, &data_lists.bounding.maxLat, &data_lists.bounding.maxLon);

            foundBounding = true;
        }

        else if (strncmp(line, "<link", 5) == 0)
        {
            // Determine if key labels are missed
            if (!is_link_format_correct(line))
            {
                printf("ERROR: Bad Attributes (%s)\n", filename);

                exit(EXIT_BAD_ATTRIBUTES);
            }

            LinkList *newLink = (LinkList *)malloc(sizeof(LinkList));

            sscanf(line, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf", &newLink->data.id, &newLink->data.node1, &newLink->data.node2, &newLink->data.way, &newLink->data.length, &newLink->data.veg, &newLink->data.arch, &newLink->data.land, &newLink->data.speed);

            foundLink = true;

            // Read POI
            char *poi_start = strstr(line, "POI=");

            if (poi_start)
            {
                poi_start += 4; 

                int poi_count = 0;

                // Count the number of POIs
                for (char *c = poi_start; *c != ';'; c++)
                {
                    if (*c == ',') poi_count++;
                }

                // Allocate memory for the POI array
                newLink->data.POI = (int *)malloc(poi_count * sizeof(int));

                newLink->data.poi_count = poi_count;

                // Read the POI values
                for (int i = 0; i < poi_count; i++)
                {
                    sscanf(poi_start, "%d,", &newLink->data.POI[i]);

                    poi_start = strchr(poi_start + 1, ',') + 1;
                }
            }

            else
            {
                newLink->data.POI = NULL;
                
                newLink->data.poi_count = 0;
            }

            newLink->next = data_lists.linkList;

            data_lists.linkList = newLink;

            data_lists.link_count++;
        }

        else if (strncmp(line, "<node", 5) == 0)
        {
            // Determine if key labels are missed
            if (!is_node_format_correct(line))
            {
                printf("ERROR: Bad Attributes (%s)\n", filename);

                exit(EXIT_BAD_ATTRIBUTES);
            }
            
            NodeList *newNode = (NodeList *)malloc(sizeof(NodeList));

            sscanf(line, "<node id=%d lat=%lf lon=%lf", &newNode->data.id, &newNode->data.lat, &newNode->data.lon);

            foundNode = true;

            newNode->next = data_lists.nodeList;

            data_lists.nodeList = newNode;

            data_lists.node_count++;
        }

        // Process the rest of the file
        else if (strncmp(line, "<way", 4) == 0)
        {
            do
            {
                UnparsedTag *new_unparsed = (UnparsedTag *)malloc(sizeof(UnparsedTag));

                new_unparsed->line = strdup(line);

                new_unparsed->next = NULL;

                if (data_lists.unparsedTags == NULL) 
                {
                    data_lists.unparsedTags = new_unparsed;
                }

                else 
                {
                    UnparsedTag *current = data_lists.unparsedTags;

                    while (current->next != NULL) 
                    {
                        current = current->next;
                    }
                    
                    current->next = new_unparsed;
                }

            } while (fgets(line, MAX_LINE_LENGTH, file) != NULL);
        }
    }
                
    fclose(file);

    // Determine if key labels are missed
    if (!foundBounding || !foundLink || !foundNode)
    {
        printf("ERROR: Bad Labels (%s)\n", filename);

        exit(EXIT_BAD_LABELS);
    }

    printf("READ AND STORED\n");

    return data_lists;
}