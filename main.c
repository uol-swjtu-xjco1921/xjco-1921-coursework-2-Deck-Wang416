#include "findRoute.h"
#include "checkInput.h"
#include "editAttribute.h"

int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <filename>\n", argv[0]);

        return EXIT_WITH_ERRORS;
    }

    // Check if file parsing was successful
    int fileParsing = parse_file(argv[1]);

    if (fileParsing == EXIT_WITH_ERRORS)

        return EXIT_WITH_ERRORS;

    // Store the data from the parsed file
    DataLists data_lists = store_data("data.txt");

    // Create an array of nodes and links
    Node *nodes = (Node *)malloc(data_lists.node_count * sizeof(Node));

    Link *links = (Link *)malloc(data_lists.link_count * sizeof(Link));
    
    // Convert a linked list to arrays
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

    bool valid_choice = false;

    do
    {
        printf("Choose An Option from Shortest Path(A), Fastest Path(B), Constraint Route(C) and Edit Attributes(D): ");

        char user_choice;

        scanf("%c", &user_choice);

        user_choice = tolower(user_choice);

        getchar();

        // Handle user choices
        switch (user_choice) 
        {
            // Shortest path
            case 'a':
                {
                    // Prompt user to enter the start and end node ID
                    int start_id = input_node_id("Enter The Start Node ID: ", nodes, data_lists.node_count);

                    int end_id = input_node_id("Enter The End Node ID: ", nodes, data_lists.node_count);

                    // Find the index of the start and end node in the arrays
                    int start_index = find_node_index(nodes, data_lists.node_count, start_id);

                    int end_index = find_node_index(nodes, data_lists.node_count, end_id);

                    Path path = dijkstra_algorithm(nodes, data_lists.node_count, links, data_lists.link_count, start_index, end_index, calculate_length);

                    if (path.length > 0) 
                    {
                        printf("The Shortest Path between Nodes %d and %d Is:\n", start_id, end_id);

                        for (int i = 0; i < path.length; i++) 
                        {
                            printf("%d", path.path[i].id);

                            if (i < path.length - 1) 
                            {
                                printf(" -> ");
                            }
                        }

                        printf("\n");

                        draw_map(nodes, data_lists.node_count, links, data_lists.link_count, &path);
                    } 
                    
                    free_path(&path);

                    valid_choice = true;

                    break;
                }

            // Fastest path
            case 'b':
                {
                    // Prompt user to enter the start and end node ID
                    int start_id = input_node_id("Enter The Start Node ID: ", nodes, data_lists.node_count);

                    int end_id = input_node_id("Enter The End Node ID: ", nodes, data_lists.node_count);

                    // Find the index of the start and end node in the arrays
                    int start_index = find_node_index(nodes, data_lists.node_count, start_id);

                    int end_index = find_node_index(nodes, data_lists.node_count, end_id);

                    bool speed_missing = true;

                    for (int i = 0; i < data_lists.link_count; i++)
                    {
                        if (links[i].speed != 0)
                        {
                            // Found a link with a non-zero speed
                            speed_missing = false;

                            break;
                        }
                    }
                
                    // If every links have not speed, prompt the user to input the speed limits
                    if (speed_missing)
                    {
                        printf("Usage: Add Speed Limits to The Links.\n");

                        char continue_input = 'y';

                        while (tolower(continue_input) == 'y')
                        {
                            // Prompt user to enter a link ID and corresponding speed
                            int link_id = input_link_id("Enter An Link ID: ", links, data_lists.link_count);

                            double link_speed = input_speed("Enter Speed of The Link: ");
                            
                            // Update the speed for the specified link
                            for (int i = 0; i < data_lists.link_count; i++)
                            {
                                if (links[i].id == link_id)
                                {
                                    links[i].speed = link_speed;
                                    
                                    break;
                                }
                            }

                            printf("Enter 'Y' to Continue, Enter Any Other Key to Finish: ");

                            scanf("%c", &continue_input);

                            getchar(); 
                        }
                    }

                    Path path = dijkstra_algorithm(nodes, data_lists.node_count, links, data_lists.link_count, start_index, end_index, calculate_time);

                    if (path.length > 0) 
                    {
                        printf("The Quickest Path between Nodes %d and %d Is:\n", start_id, end_id);

                        for (int i = 0; i < path.length; i++) 
                        {
                            printf("%d", path.path[i].id);

                            if (i < path.length - 1) 
                            {
                                printf(" -> ");
                            }
                        }

                        printf("\n");

                        draw_map(nodes, data_lists.node_count, links, data_lists.link_count, &path);
                    } 

                    free_path(&path);

                    valid_choice = true;
                    
                    break;
                }
            
            // Constraint route (Coming soon!)
            case 'c':

                printf("Coming Soon!\n");

                valid_choice = true;

                break;

            // Attributes edition
            case 'd':
                {
                    // Loop until a valid choice for editing attributes is provided
                    bool correct_choice = false;

                    do
                    {
                        printf("Choose A Type of Element to Modify from Link(1) and Node(2): ");

                        char user_selection;

                        scanf("%c", &user_selection);

                        getchar();

                        // Handle user choices for editing attributes
                        switch (user_selection) 
                        {
                            // Edit link attributes
                            case '1':
                            {
                                int link_id = input_link_id("Enter The Link ID: ", links, data_lists.link_count);

                                printf("Link Attributes: length, veg, arch, land, speed.\n");

                                printf("Enter The Attribute to Modify: ");

                                char attribute[10];

                                fgets(attribute, sizeof(attribute), stdin);

                                strtok(attribute, "\n");

                                // Check if the attribute is valid
                                while (strcmp(attribute, "length") != 0 && strcmp(attribute, "veg") != 0 &&
                                    strcmp(attribute, "arch") != 0 && strcmp(attribute, "land") != 0 &&
                                    strcmp(attribute, "speed") != 0)
                                {
                                    printf("Usage: Enter An Existing Attribute.\n");

                                    printf("Enter The Attribute: ");

                                    fgets(attribute, sizeof(attribute), stdin);

                                    strtok(attribute, "\n");
                                }

                                // Input new attribute value
                                double new_value = input_speed("Enter The New Value: ");

                                // Update link attribute in the data file
                                update_link_attribute(argv[1], nodes, data_lists, links, data_lists.link_count, link_id, attribute, new_value);

                                correct_choice = true;

                                break; 
                            }

                            // Edit node attributes
                            case '2':
                                {
                                    int node_id = input_node_id("Enter The Node ID: ", nodes, data_lists.node_count);

                                    printf("Node Attributes: lat, lon\n");

                                    printf("Enter The Attribute to Modify: ");

                                    char attribute[10];

                                    fgets(attribute, sizeof(attribute), stdin);

                                    strtok(attribute, "\n");

                                    // Check if the attribute is valid
                                    while (strcmp(attribute, "lat") != 0 && strcmp(attribute, "lon") != 0)
                                    {
                                        printf("Usage: Enter An Existing Attribute.\n");

                                        printf("Enter The Attribute: ");

                                        fgets(attribute, sizeof(attribute), stdin);
                                        
                                        strtok(attribute, "\n");
                                    }

                                    double new_value;

                                    // Input new attribute value
                                    if (strcmp(attribute, "lat") == 0)
                                    {
                                        new_value = input_coordinate("Enter The New Value: ", data_lists.bounding.minLat, data_lists.bounding.maxLat);
                                    }

                                    else
                                    {
                                        new_value = input_coordinate("Enter The New Value: ", data_lists.bounding.minLon, data_lists.bounding.maxLon);
                                    }

                                    // Update node attribute in the data file
                                    update_node_attribute(argv[1], nodes, data_lists, links, data_lists.node_count, node_id, attribute, new_value);

                                    correct_choice = true;

                                    break;            
                                }

                            default:
                        
                                {
                                    printf("Usage: Enter Either 1 or 2.\n");

                                    break;                           
                                }
                        }

                    } while (!correct_choice);

                    printf("EDITED\n");

                    valid_choice = true;

                    break;
                }

            default:

                printf("Usage: Enter Either A, B or C.\n");

                break;
        }

    } while (!valid_choice);
    
    // Free allocated memory
    free(nodes);

    free(links);
    
    free_list(data_lists.nodeList, NODE_LIST);

    free_list(data_lists.linkList, LINK_LIST);

    return EXIT_WITHOUT_ERRORS;
}