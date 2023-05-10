#include "readMap.h"

int parse_file(const char *filename) 
{
    Node *nodes = NULL;
    int node_count = 0;

    Link *links = NULL;
    int link_count = 0;

    // Add bounding variable to store bounding information
    Bounding bounding = {0};
    
    FILE *file = fopen(filename, "r");
    
    // Check if the file can be opened successfully
    if (file == NULL) 
    {
        printf("ERROR: Bad File Name (%s)\n", filename);

        return EXIT_WITH_ERRORS;
    }

    char line[MAX_LINE_LENGTH];

    // Read the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) 
    {
        if (strncmp(line, "<bounding", 9) == 0) 
        {
            sscanf(line, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>", &bounding.minLat, &bounding.minLon, &bounding.maxLat, &bounding.maxLon);
        }

        else if (strncmp(line, "<node", 5) == 0) 
        {
            Node node = {0};

            // Extract node data from the line
            sscanf(line, "<node id=%d lat=%lf lon=%lf", &node.id, &node.lat, &node.lon);

            // Resize the nodes array to accommodate the new node
            nodes = realloc(nodes, sizeof(Node) * (node_count + 1));

            // Add the new node to the nodes array
            nodes[node_count] = node;

            node_count++;
        } 
        
        else if (strncmp(line, "<link", 5) == 0) 
        {
            Link link = {0};

            // Extract link data from the line
            sscanf(line, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf", &link.id, &link.node1, &link.node2, &link.way, &link.length, &link.veg, &link.arch, &link.land, &link.speed);

            // Resize the links array to accommodate the new link
            links = realloc(links, sizeof(Link) * (link_count + 1));

            // Add the new link to the links array
            links[link_count] = link;
            
            link_count++;
        }
    }

    fclose(file);

    FILE *output_file = fopen("data.txt", "w");

    // Write bounding data on the first line
    fprintf(output_file, "<bounding minLat=%lf minLon=%lf maxLat=%lf maxLon=%lf /bounding>\n", bounding.minLat, bounding.minLon, bounding.maxLat, bounding.maxLon);

    // Write nodes data
    for (int i = 0; i < node_count; i++)
    {
        fprintf(output_file, "<node id=%d lat=%lf lon=%lf /node>\n", nodes[i].id, nodes[i].lat, nodes[i].lon);
    }

    // Write links data
    for (int i = 0; i < link_count; i++)
    {
        fprintf(output_file, "<link id=%d node=%d node=%d way=%d length=%lf veg=%lf arch=%lf land=%lf speed=%lf /link>\n", links[i].id, links[i].node1, links[i].node2, links[i].way, links[i].length, links[i].veg, links[i].arch, links[i].land, links[i].speed);
    }

    fclose(output_file);

    // Free allocated memory
    free(nodes);

    free(links);

    printf("PARSED\n");

    return EXIT_WITHOUT_ERRORS;
}