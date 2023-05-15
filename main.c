#include "findRoute.h"
#include "checkInput.h"
#include "editAttribute.h"

#define MAX_NODES 100

int main(int argc, char **argv) 
{
    if(argc == 1)
    {
        printf("Usage: %s inputFile.map\n", argv[0]);

        exit(EXIT_NO_ERRORS);
    }

    else if (argc != 2) 
    {
        printf("ERROR: Bad Argument Count\n");

        exit(EXIT_WRONG_ARG_COUNT);
    }

    // Read and store the data from the original file
    DataLists data_lists = parse_and_store_data(argv[1]);

    // Create an array of nodes and links
    Node *nodes = (Node *)malloc(data_lists.node_count * sizeof(Node));

    if (nodes == NULL)
    {
        printf("ERROR: Nodes Malloc Failed\n");

        exit(EXIT_MALLOC_FAILED);
    }

    Link *links = (Link *)malloc(data_lists.link_count * sizeof(Link));

    if (links == NULL)
    {
        printf("ERROR: Links Malloc Failed\n");

        exit(EXIT_MALLOC_FAILED);
    }
    
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

                    int end_id;

                    do 
                    {
                        end_id = input_node_id("Enter The End Node ID: ", nodes, data_lists.node_count);

                        // Check if the end node ID and start node ID are the same
                        if (start_id == end_id)
                        {
                            printf("Usage: Enter An End Node ID Different from The Start Node ID\n");
                        }

                    } while (start_id == end_id);

                    // Find the index of the start and end node in the arrays
                    int start_index = find_node_index(nodes, data_lists.node_count, start_id);

                    int end_index = find_node_index(nodes, data_lists.node_count, end_id);

                    Path path = dijkstra_algorithm(nodes, data_lists.node_count, links, data_lists.link_count, start_index, end_index, calculate_length);

                    if (path.length == -1)
                    {
                        printf("No Shortest Path Found between Nodes %d and %d\n", start_id, end_id);
                    }

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

                    int end_id;

                    do 
                    {
                        end_id = input_node_id("Enter The End Node ID: ", nodes, data_lists.node_count);

                        // Check if the end node ID and start node ID are the same
                        if (start_id == end_id)
                        {
                            printf("Usage: Enter An End Node ID Different from The Start Node ID\n");
                        }
                        
                    } while (start_id == end_id);

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
                        printf("Usage: Add Speed Limits to The Links\n");

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

                    if (path.length == -1)
                    {
                        printf("No Quickest Path Found between Nodes %d and %d\n", start_id, end_id);
                    }
                    
                    else if (path.length > 0) 
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

                    // Save the updated data to original and parsed files
                    save_data(argv[1], nodes, links, &data_lists);

                    free_path(&path);

                    valid_choice = true;
                    
                    break;
                }

            // Constraint route
            case 'c':
                {
                    // Prompt user to enter the start and end node ID
                    int start_id = input_node_id("Enter The Start Node ID: ", nodes, data_lists.node_count);

                    int end_id;

                    do 
                    {
                        end_id = input_node_id("Enter The End Node ID: ", nodes, data_lists.node_count);

                        // Check if the end node ID and start node ID are the same
                        if (start_id == end_id)
                        {
                            printf("Usage: Enter An End Node ID Different from The Start Node ID\n");
                        }
                        
                    } while (start_id == end_id);

                    bool correct_choice = false;

                    // Initialize an array to store the intermediate node IDs
                    int intermediate_nodes[MAX_NODES];

                    int num_intermediate_nodes = 0;

                    Path constrained_path;

                    do
                    {
                        // Prompt user to choose between location and POI
                        printf("Choose An Option from Passing A Given Location(L) and POI(P): ");

                        char constraint_type, input_continue; 

                        scanf("%c", &constraint_type);

                        getchar(); 

                        constraint_type = toupper(constraint_type);

                        switch (constraint_type) 
                        {
                            case 'L':
                                {
                                    // Prompt user to enter intermediate node IDs
                                    input_continue = 'y';

                                    while (tolower(input_continue) == 'y') 
                                    {
                                        intermediate_nodes[num_intermediate_nodes] = input_node_id("Enter An Intermediate Node ID: ", nodes, data_lists.node_count);

                                        num_intermediate_nodes++;

                                        printf("Enter 'Y' to Continue, Enter Any Other Key to Finish: ");

                                        scanf("%c", &input_continue);

                                        getchar(); 
                                    }

                                    // Calculate the shortest route pass a given location
                                    constrained_path = shortest_path_with_positions(nodes, data_lists.node_count, links, data_lists.link_count, start_id, end_id, intermediate_nodes, num_intermediate_nodes);

                                    correct_choice = true;

                                    break;
                                }

                            case 'P':
                                {
                                    input_continue = 'y';

                                    while (tolower(input_continue) == 'y')
                                    {
                                        // Prompt user to enter a POI ID
                                        intermediate_nodes[num_intermediate_nodes] = input_POI("Enter An POI ID: ", links, data_lists.link_count);

                                        num_intermediate_nodes++;

                                        printf("Enter 'Y' to Continue, Enter Any Other Key to Finish: ");

                                        scanf("%c", &input_continue);

                                        getchar(); 
                                    }
                                    
                                    // Calculate the shortest route passing through the given POIs
                                    constrained_path = shortest_path_with_pois(nodes, data_lists.node_count, links, data_lists.link_count, start_id, end_id, intermediate_nodes, num_intermediate_nodes);

                                    correct_choice = true;

                                    break;
                                }                     

                            default:

                                printf("Usage: Enter Either L or P\n");

                                break;
                        }

                    } while(!correct_choice);

                    if (constrained_path.length == 0) 
                    {
                        printf("No Shortest Path with Constraints Found between Nodes %d and %d\n", start_id, end_id);
                    }

                    else if (constrained_path.length > 0) 
                    {
                        printf("The Shortest Path with Constraints between Nodes %d and %d Is:\n", start_id, end_id);

                        for (int i = 0; i < constrained_path.length; i++) 
                        {
                            printf("%d", constrained_path.path[i].id);

                            if (i < constrained_path.length - 1) 
                            {
                                printf(" -> ");
                            }
                        }

                        printf("\n");

                        draw_map(nodes, data_lists.node_count, links, data_lists.link_count, &constrained_path);
                    }

                    free_path(&constrained_path);

                    valid_choice = true;

                    break;
                }

            // Attributes edition
            case 'd':
                {
                    // Loop until a valid choice for editing attributes is provided
                    bool correct_choice = false;

                    do
                    {
                        printf("Choose A Type of Element to Modify from Link(1), Node(2) and POI(3): ");

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

                                printf("Link Attributes: length, veg, arch, land, speed\n");

                                printf("Enter The Attribute to Modify: ");

                                char attribute[10];

                                fgets(attribute, sizeof(attribute), stdin);

                                strtok(attribute, "\n");

                                // Check if the attribute is valid
                                while (strcmp(attribute, "length") != 0 && strcmp(attribute, "veg") != 0 &&
                                    strcmp(attribute, "arch") != 0 && strcmp(attribute, "land") != 0 &&
                                    strcmp(attribute, "speed") != 0)
                                {
                                    printf("Usage: Enter An Existing Attribute\n");

                                    printf("Enter The Attribute: ");

                                    fgets(attribute, sizeof(attribute), stdin);

                                    strtok(attribute, "\n");
                                }

                                // Input new attribute value
                                double new_value = input_speed("Enter The New Value: ");

                                // Update link attribute in the data file
                                update_link_attribute(argv[1], nodes, links, &data_lists, link_id, attribute, new_value);

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
                                        printf("Usage: Enter An Existing Attribute\n");

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
                                    update_node_attribute(argv[1], nodes, links, &data_lists, node_id, attribute, new_value);

                                    correct_choice = true;

                                    break;            
                                }
                            
                            // Edit link POI attributes
                            case '3':
                                {
                                    int link_id = input_link_id("Enter The Link ID: ", links, data_lists.link_count);

                                    char continue_input = 'y';

                                    while (tolower(continue_input) == 'y')
                                    {
                                        // Input new POI id
                                        printf("Enter The New POI ID: ");

                                        int new_poi_id;

                                        scanf("%d", &new_poi_id);

                                        getchar();

                                        // Update link POI attribute in the data file
                                        update_poi_attribute(argv[1], nodes, links, &data_lists, link_id, new_poi_id);

                                        printf("Enter 'Y' to Continue, Enter Any Other Key to Finish: ");

                                        scanf("%c", &continue_input);

                                        getchar(); 
                                    }

                                    correct_choice = true;

                                    break; 
                                }

                            default:
                        
                                {
                                    printf("Usage: Enter Either 1 or 2\n");

                                    break;                           
                                }
                        }

                    } while (!correct_choice);

                    printf("EDITED\n");

                    valid_choice = true;

                    break;
                }

            default:

                printf("Usage: Enter Either A, B or C\n");

                break;
        }

    } while (!valid_choice);
    
    // Free allocated memory
    free(nodes);

    free(links);

    free_unparsed_tags(data_lists.unparsedTags);
    
    free_list(data_lists.nodeList, NODE_LIST);

    free_list(data_lists.linkList, LINK_LIST);

    return EXIT_NO_ERRORS;
}