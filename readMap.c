#include "readMap.h"

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

        return data_lists;
    }

    char line[MAX_LINE_LENGTH];

    // Read the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        if (strncmp(line, "<bounding", 9) == 0)
        {
            sscanf(line, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>", &data_lists.bounding.minLat, &data_lists.bounding.minLon, &data_lists.bounding.maxLat, &data_lists.bounding.maxLon);
        }

        else if (strncmp(line, "<node", 5) == 0)
        {
            NodeList *newNode = (NodeList *)malloc(sizeof(NodeList));

            sscanf(line, "<node id=%d lat=%lf lon=%lf", &newNode->data.id, &newNode->data.lat, &newNode->data.lon);

            newNode->next = data_lists.nodeList;

            data_lists.nodeList = newNode;

            data_lists.node_count++;
        }

        else if (strncmp(line, "<link", 5) == 0)
        {
            LinkList *newLink = (LinkList *)malloc(sizeof(LinkList));

            sscanf(line, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf", &newLink->data.id, &newLink->data.node1, &newLink->data.node2, &newLink->data.way, &newLink->data.length, &newLink->data.veg, &newLink->data.arch, &newLink->data.land, &newLink->data.speed);

            newLink->next = data_lists.linkList;

            data_lists.linkList = newLink;

            data_lists.link_count++;
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

    printf("READ AND STORED\n");

    return data_lists;
}