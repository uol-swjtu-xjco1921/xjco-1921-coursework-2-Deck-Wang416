#include "findRoute.h"

int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <filename>\n", argv[0]);

        return EXIT_FAILURE;
    }

    parse_file(argv[1]);

    DataLists data_lists = store_data("data.txt");

    // Create an array of nodes and links
    Node *nodes = (Node *)malloc(data_lists.node_count * sizeof(Node));

    Link *links = (Link *)malloc(data_lists.link_count * sizeof(Link));
    
    // Convert a linked list to an array
    NodeList *node_iter = data_lists.nodeList;

    for (int i = 0; i < data_lists.node_count; i++)
    {
        nodes[i] = node_iter->data;

        node_iter = node_iter->next;
    }

    LinkList *link_iter = data_lists.linkList;
    
    for (int i = 0; i < data_lists.link_count; i++)
    {
        links[i] = link_iter->data;

        link_iter = link_iter->next;
    }

    int start_id = 1601125096; 
    
    int end_id = 1187324692;  

    Path path = dijkstra_algorithm(nodes, data_lists.node_count, links, data_lists.link_count, start_id, end_id);

    if (path.length > 0) 
    {
        printf("Shortest path between nodes %d and %d:\n", start_id, end_id);

        for (int i = 0; i < path.length; i++) 
        {
            printf("%d", path.path[i].id);

            if (i < path.length - 1) 
            {
                printf(" -> ");
            }
        }

        printf("\n");

    } 
    
    else 
    {
        printf("No path found between nodes %d and %d\n", start_id, end_id);
    }

    draw_map(nodes, data_lists.node_count, links, data_lists.link_count, &path);

    free_path(&path);
    
    // Free allocated memory
    free(nodes);

    free(links);
    
    free_list(data_lists.nodeList, NODE_LIST);

    free_list(data_lists.linkList, LINK_LIST);

    return EXIT_SUCCESS;
}