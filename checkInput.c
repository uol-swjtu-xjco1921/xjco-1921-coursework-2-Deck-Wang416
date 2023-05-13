#include "checkInput.h"

// Check if the node ID is reasonable
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

// Check if the link ID is reasonable
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
            char *endptr;

            strtol(input_buffer, &endptr, 10);

            // Check for extra characters
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

// // Check if the speed is reasonable
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
        if (input_result != 1 || speed <= 0)
        {
            printf("Usage: Enter A Positive Number.\n");

            valid_speed = false;
        }

        else
        {
            char *endptr;

            strtod(input_buffer, &endptr);

            // Check for extra characters
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

// Check if the Longitude and latitude are reasonable
double input_coordinate(const char *prompt, double min_value, double max_value)
{
    char input_buffer[100];

    double attribute_value;

    int input_result;

    bool valid_input;

    do
    {
        printf("%s", prompt);

        fgets(input_buffer, sizeof(input_buffer), stdin);

        input_result = sscanf(input_buffer, "%lf", &attribute_value);

        // Check for wrong data type
        if (input_result != 1 )
        {
            printf("Usage: Enter A Numeric Value.\n");

            valid_input = false;
        }

        else
        {        
            char *endptr;

            strtod(input_buffer, &endptr);

            // Check for extra characters
            if (*endptr != '\n' && *endptr != '\0')
            {
                printf("Usage: Enter A Numeric Value With No Extra Characters.\n");

                valid_input = false;
            }

            // Check for Values beyond the reasonable range
            else if (attribute_value < min_value || attribute_value > max_value)
            {
                printf("Usage: Enter A Valid Value Between %lf And %lf.\n", min_value, max_value);

                valid_input = false;
            }

            else
            {
                valid_input = true;
            }
        }

    } while (!valid_input);

    return attribute_value;
}

// Check if the POI is reasonable
int input_POI(const char *prompt, Link *links, int num_links)
{
    char input_buffer[100];

    int poi_id, input_result;

    bool has_decimal_point, valid_poi_id;

    do
    {
        printf("%s", prompt);

        fgets(input_buffer, sizeof(input_buffer), stdin);

        // Check if the input contains a decimal point
        has_decimal_point = strchr(input_buffer, '.') != NULL;

        input_result = sscanf(input_buffer, "%d", &poi_id);

        // Check if the input is a non-negative integer
        if (input_result != 1 || has_decimal_point || poi_id < 0)
        {
            printf("Usage: Enter A Non-negative Integer.\n");

            valid_poi_id = false;
        }

        else
        {
            // Check for extra characters
            char *endptr;

            strtol(input_buffer, &endptr, 10);

            if (*endptr != '\n' && *endptr != '\0')
            {
                printf("Usage: Enter An Integer With No Extra Characters.\n");

                valid_poi_id = false;
            }

            else
            {
                valid_poi_id = false;

                for (int i = 0; i < num_links; i++)
                {
                    for (int j = 0; j < links[i].poi_count; j++)
                    {
                        if (links[i].POI[j] == poi_id)
                        {
                            valid_poi_id = true;

                            break;
                        }
                    }

                    if (valid_poi_id)
                    {
                        break;
                    }
                }

                // Check if the POI ID is valid
                if (!valid_poi_id)
                {
                    printf("Usage: Enter An Existing POI ID.\n");
                }
            }
        }

    // Repeat until a valid POI ID is entered
    } while (!valid_poi_id);

    return poi_id;
}
