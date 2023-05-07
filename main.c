#include "findRoute.h"

int input_node_id(const char *prompt, Node *nodes, int num_nodes)
{
    char input_buffer[100];

    int node_id, input_result;

    bool has_decimal_point, valid_node_id;

    do
    {
        printf("%s", prompt);

        fgets(input_buffer, sizeof(input_buffer), stdin);

        // Check if the input contains a decimal point
        has_decimal_point = strchr(input_buffer, '.') != NULL;

        input_result = sscanf(input_buffer, "%d", &node_id);

        // Check if the input is an integer
        if (input_result != 1 || has_decimal_point)
        {
            printf("Usage: Enter An Integer.\n");

            valid_node_id = false;
        }

        else
        {
            // Check for extra characters
            char *endptr;

            strtol(input_buffer, &endptr, 10);

            if (*endptr != '\n' && *endptr != '\0')
            {
                printf("Usage: Enter An Integer With No Extra Characters.\n");

                valid_node_id = false;
            }

            else
            {
                int node_index = find_node_index(nodes, num_nodes, node_id);

                valid_node_id = node_index != -1;

                // Check if the node ID is valid
                if (!valid_node_id)
                {
                    printf("Usage: Enter An Existing Node ID.\n");
                }
            }
        }

    // Repeat until a valid node ID is entered
    } while (input_result != 1 || has_decimal_point || !valid_node_id);

    return node_id;
}

int input_link_id(const char *prompt, Link *links, int num_links)
{
    char input_buffer[100];

    int link_id, input_result;

    bool has_decimal_point, valid_link_id;

    do
    {
        printf("%s", prompt);

        fgets(input_buffer, sizeof(input_buffer), stdin);

        // Check if the input contains a decimal point
        has_decimal_point = strchr(input_buffer, '.') != NULL;

        input_result = sscanf(input_buffer, "%d", &link_id);

        // Check if the input is an integer
        if (input_result != 1 || has_decimal_point)
        {
            printf("Usage: Enter An Integer.\n");

            valid_link_id = false;
        }

        else
        {
            // Check for extra characters
            char *endptr;

            strtol(input_buffer, &endptr, 10);

            if (*endptr != '\n' && *endptr != '\0')
            {
                printf("Usage: Enter An Integer With No Extra Characters.\n");

                valid_link_id = false;
            }

            else
            {
                int link_index = -1;

                for (int i = 0; i < num_links; i++)
                {
                    if (links[i].id == link_id)
                    {
                        link_index = i;

                        break;
                    }
                }

                valid_link_id = link_index != -1;

                // Check if the link ID is valid
                if (!valid_link_id)
                {
                    printf("Usage: Enter An Existing Link ID.\n");
                }
            }
        }

    // Repeat until a valid link ID is entered
    } while (input_result != 1 || has_decimal_point || !valid_link_id);

    return link_id;
}

double input_speed(const char *prompt)
{
    char input_buffer[100];

    double speed;

    int input_result;

    bool valid_speed;

    do
    {
        printf("%s", prompt);

        fgets(input_buffer, sizeof(input_buffer), stdin);

        input_result = sscanf(input_buffer, "%lf", &speed);

        // Check if the input is a positive number
        if (input_result != 1 || speed < 0)
        {
            printf("Usage: Enter A Positive Number.\n");

            valid_speed = false;
        }

        else
        {
            // Check for extra characters
            char *endptr;

            strtod(input_buffer, &endptr);

            if (*endptr != '\n' && *endptr != '\0')
            {
                printf("Usage: Enter A Positive Number With No Extra Characters.\n");

                valid_speed = false;
            }

            else
            {
                valid_speed = true;
            }
        }

    // Repeat until a valid speed is entered
    } while (!valid_speed);

    return speed;
}

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
        printf("Choose An Option from Shortest Path(A), Fastest Path(B) or Constraint Route(C): ");

        char user_choice;

        scanf("%c", &user_choice);

        getchar();

        user_choice = tolower(user_choice);

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
            
            // Constraint route (coming soon)
            case 'c':

                printf("Coming Soon!\n");

                valid_choice = true;

                break;

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