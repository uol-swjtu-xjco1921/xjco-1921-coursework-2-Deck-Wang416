#include "/opt/homebrew/include/SDL2/SDL.h" // #include <SDL2/SDL.h>
#include "visualization.h"

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

    draw_map(nodes, data_lists.node_count, links, data_lists.link_count);
    
    // Free allocated memory
    free(nodes);

    free(links);
    
    free_list(data_lists.nodeList, NODE_LIST);

    free_list(data_lists.linkList, LINK_LIST);

    return EXIT_SUCCESS;
}