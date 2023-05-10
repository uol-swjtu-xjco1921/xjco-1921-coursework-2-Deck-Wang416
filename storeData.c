#include "storeData.h"

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

// Read and store the data in linkList and nodeList
DataLists store_data(const char *filename)
{
    DataLists data_lists = {NULL, NULL, {0, 0, 0, 0}, 0, 0};
    
    FILE* file = fopen(filename, "r");

    // Check if the file can be opened successfully
    if (file == NULL) 
    {
        printf("ERROR: Bad File Name (%s)\n", filename);

        return data_lists;
    }
   
    char line[1024];

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "<bounding", 9) == 0)
        {
            // Extract bounding data from the line
            sscanf(line, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf", &data_lists.bounding.minLat, &data_lists.bounding.minLon, &data_lists.bounding.maxLat, &data_lists.bounding.maxLon);
        }

        if (strncmp(line, "<link", 5) == 0) 
        {
            LinkList *newLink = (LinkList *)malloc(sizeof(LinkList));

            // Extract link data from the line
            sscanf(line, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf", &newLink->data.id, &newLink->data.node1, &newLink->data.node2, &newLink->data.way, &newLink->data.length, &newLink->data.veg, &newLink->data.arch, &newLink->data.land, &newLink->data.speed);

            // Add the new link to the linkList
            newLink->next = data_lists.linkList;

            data_lists.linkList = newLink;
        } 

        if (strncmp(line, "<node", 5) == 0)
        {
            NodeList *newNode = (NodeList *)malloc(sizeof(NodeList));

            // Extract node data from the line
            sscanf(line, "<node id=%d lat=%lf lon=%lf", &newNode->data.id, &newNode->data.lat, &newNode->data.lon);

            // Add the new node to the nodeList
            newNode->next = data_lists.nodeList;

            data_lists.nodeList = newNode;
        }
    }

    fclose(file);

    // Calculate the number of nodes and links
    NodeList *node_iter = data_lists.nodeList;

    while (node_iter)
    {
        data_lists.node_count++;

        node_iter = node_iter->next;
    }

    LinkList *link_iter = data_lists.linkList;

    while (link_iter)
    {
        data_lists.link_count++;

        link_iter = link_iter->next;
    }

    printf("STORED\n");

    return data_lists;
}