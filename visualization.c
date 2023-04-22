#include "visualization.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Map Visualization"

int find_node_index(Node *nodes, int node_count, int id) 
{
    for (int i = 0; i < node_count; i++) 
    {
        if (nodes[i].id == id) 
        {
            return i;
        }
    }

    return EXIT_FAILURE;
}

// Flip the y-coordinate
int flip_y(int y) 
{
    return WINDOW_HEIGHT - y;
}

// Draw the map using the nodes and links provided
void draw_map(Node *nodes, int node_count, Link *links, int link_count) 
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("ERROR: Initialize SDL Failed(%s)\n", SDL_GetError());

        return;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) 
    {
        printf("ERROR: Create Window Failed (%s)\n", SDL_GetError());

        return;
    }

    // Create a Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) 
    {
        printf("ERROR: Create Renderer Failed (%s)\n", SDL_GetError());

        return;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) 
    {
        printf("ERROR: Initialize SDL_ttf Failed (%s)\n", TTF_GetError());

        return;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 16);

    if (font == NULL) 
    {
        printf("ERROR: Load Font Failed (%s)\n", TTF_GetError());

        TTF_Quit();

        return;
    }

    // Set scaling factor
    double scale_factor = 0.8;

    // Main loop
    SDL_bool running = SDL_TRUE;

    while (running) 
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = SDL_FALSE;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer);

        // Calculate the maximum and minimum values of longitude and latitude
        double min_lat = nodes[0].lat, max_lat = nodes[0].lat;

        double min_lon = nodes[0].lon, max_lon = nodes[0].lon;

        for (int i = 0; i < node_count; i++) 
        {
            if (nodes[i].lat < min_lat) min_lat = nodes[i].lat;

            if (nodes[i].lat > max_lat) max_lat = nodes[i].lat;

            if (nodes[i].lon < min_lon) min_lon = nodes[i].lon;

            if (nodes[i].lon > max_lon) max_lon = nodes[i].lon;
        }

        // Round min_lon and max_lon to the nearest standardized value
        min_lon = floor(min_lon * 1000.0) / 1000.0;

        max_lon = ceil(max_lon * 1000.0) / 1000.0;

        // Add the new lines for rounding min_lat and max_lat
        min_lat = ceil(min_lat * 1000.0) / 1000.0;

        max_lat = ceil(max_lat * 1000.0) / 1000.0;

        // Calculate the latitude and longitude range
        double lat_range = max_lat - min_lat;

        double lon_range = max_lon - min_lon;

        // Leave some top margin to prevent the map from overflowing at the top
        double scale_lat = ((WINDOW_HEIGHT - 10) / lat_range) * scale_factor;

        // Leave some right margin to prevent the map from overflowing on the right side
        double scale_lon = ((WINDOW_WIDTH + 10) / lon_range) * scale_factor;

        double offset_lat = 50 - min_lat * scale_lat;

        double offset_lon = 50 - min_lon * scale_lon + 50;

        // Draw nodes
        for (int i = 0; i < node_count; i++)
        {
            bool isConnected = false;

            for (int j = 0; j < link_count; j++)
            {
                if (links[j].node1 == nodes[i].id || links[j].node2 == nodes[i].id)
                {
                    isConnected = true;

                    break;
                }
            }

            if (isConnected)
            {
                SDL_Rect node_rect;

                // Convert longitude and latitude coordinates to pixel coordinates
                node_rect.x = (int)(nodes[i].lon * scale_lon + offset_lon);

                node_rect.y = flip_y((int)(nodes[i].lat * scale_lat + offset_lat));

                node_rect.w = 5;

                node_rect.h = 5;

                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

                SDL_RenderFillRect(renderer, &node_rect);
            }
        }

        // Draw connection lines
        for (int i = 0; i < link_count; i++)
        {
            int node1_index = find_node_index(nodes, node_count, links[i].node1);

            int node2_index = find_node_index(nodes, node_count, links[i].node2);

            if (node1_index != -1 && node2_index != -1)
            {
                SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);

                SDL_RenderDrawLine(renderer,
                                (int)(nodes[node1_index].lon * scale_lon + offset_lon),
                                flip_y((int)(nodes[node1_index].lat * scale_lat + offset_lat)),
                                (int)(nodes[node2_index].lon * scale_lon + offset_lon),
                                flip_y((int)(nodes[node2_index].lat * scale_lat + offset_lat)));
            }
        }

        // Draw x and y axes
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        int xAxisOffset = 50;

        SDL_RenderDrawLine(renderer, 50 + xAxisOffset, flip_y(50), (int)((max_lon * scale_lon) + offset_lon), flip_y(50));

        SDL_RenderDrawLine(renderer, 50 + xAxisOffset, flip_y(50), 50 + xAxisOffset, flip_y((int)((max_lat * scale_lat) + offset_lat)));

        // Draw right side y-axis
        SDL_RenderDrawLine(renderer, (int)((max_lon * scale_lon) + offset_lon), flip_y(50), (int)((max_lon * scale_lon) + offset_lon), flip_y((int)((max_lat * scale_lat) + offset_lat)));

        // Draw top side x-axis
        SDL_RenderDrawLine(renderer, 50 + xAxisOffset, flip_y((int)((max_lat * scale_lat) + offset_lat)), (int)((max_lon * scale_lon) + offset_lon), flip_y((int)((max_lat * scale_lat) + offset_lat)));

        int x_ticks = 5;

        int y_ticks = 11;

        double x_tick_interval = round(((max_lon - min_lon) / x_ticks) * 1000.0) / 1000.0;

        double y_tick_interval = round(((max_lat - min_lat) / y_ticks) * 1000.0) / 1000.0;

        // Draw tick marks and labels for x and y axes
        for (int i = 0; i <= x_ticks; i++) 
        {
            tickmarks_values(renderer, font, true, i, x_ticks, min_lon, max_lon, scale_lon, offset_lon, xAxisOffset); 
        }

        for (int j = 0; j <= y_ticks; j++) 
        {
            tickmarks_values(renderer, font, false, j, y_ticks, min_lat, max_lat, scale_lat, offset_lat, xAxisOffset);
        }

        // Draw tick marks for the right side y-axis
        for (int j = 0; j <= y_ticks; j++) 
        {
            int pos = (int)(round((min_lat + j * y_tick_interval) * 1000.0) / 1000.0 * scale_lat + offset_lat);

            SDL_RenderDrawLine(renderer, (int)((max_lon * scale_lon) + offset_lon - 5), flip_y(pos), (int)((max_lon * scale_lon) + offset_lon), flip_y(pos));
        }

        // Draw tick marks for the top side x-axis
        for (int i = 0; i <= x_ticks; i++) 
        {
            int pos = (int)(round((min_lon + i * x_tick_interval) * 1000.0) / 1000.0 * scale_lon + offset_lon);

            SDL_RenderDrawLine(renderer, pos, flip_y((int)((max_lat * scale_lat) + offset_lat)), pos, flip_y((int)((max_lat * scale_lat) + offset_lat - 5)));
        }

        // Draw legend
        int legend_x = WINDOW_WIDTH - 135;

        int legend_y = (int)((max_lat * scale_lat) + offset_lat) - 15;

        // Draw link line
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);

        SDL_RenderDrawLine(renderer, legend_x, flip_y(legend_y), legend_x + 50, flip_y(legend_y));

        // Draw node
        SDL_Rect node_rect;

        // Center the node horizontally
        node_rect.x = legend_x + 22; 

        // Center the node vertically
        node_rect.y = flip_y(legend_y) - 2; 

        node_rect.w = 5;

        node_rect.h = 5;

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_RenderFillRect(renderer, &node_rect);

        // Draw text "map.out"
        SDL_Surface *surface = TTF_RenderText_Solid(font, "'map.out'", (SDL_Color){0, 0, 0, 255});

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        int text_width, text_height;

        SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);

        SDL_Rect text_rect = {legend_x - text_width - 10, flip_y(legend_y) - text_height / 2, text_width, text_height};

        SDL_RenderCopy(renderer, texture, NULL, &text_rect);

        SDL_FreeSurface(surface);
        
        SDL_DestroyTexture(texture);

        SDL_RenderPresent(renderer);
    }
    
    // Destroy the renderer and window, exit SDL
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();

    printf("VISUALIZED\n");
}

void tickmarks_values(SDL_Renderer *renderer, TTF_Font *font, bool isXAxis, int index, int total_ticks, double minValue, double maxValue, double scale, double offset, int axisOffset) 
{
    // Calculate tick interval
    double tick_interval = round(((maxValue - minValue) / total_ticks) * 1000.0) / 1000.0;

    // Calculate tick value
    double value = round((minValue + index * tick_interval) * 1000.0) / 1000.0;

    // Calculate position based on value
    int pos = (int)(value * scale + offset);

    char buffer[64];

    // Format tick value with appropriate precision
    if ((int)(value * 1000) % 10 == 0) 
    {
        snprintf(buffer, sizeof(buffer), "%.2f", value);
    } 
    
    else 
    {
        snprintf(buffer, sizeof(buffer), "%.3f", value);
    }

    // Create text surface with tick value
    SDL_Surface *surface = TTF_RenderText_Solid(font, buffer, (SDL_Color){0, 0, 0, 255});

    // Create texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width, text_height;

    // Get text dimensions
    SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);

    SDL_Rect text_rect;

    if (isXAxis) 
    {
        // Draw x-axis tick line
        SDL_RenderDrawLine(renderer, pos, flip_y(50), pos, flip_y(55));

        // Set text rectangle position for x-axis
        text_rect = (SDL_Rect){pos - text_width / 2, flip_y(30) - text_height, text_width, text_height};
    } 

    else
    {
        // Draw y-axis tick line
        SDL_RenderDrawLine(renderer, 50 + axisOffset, flip_y(pos), 55 + axisOffset, flip_y(pos));

        // Set text rectangle position for y-axis
        text_rect = (SDL_Rect){30 - text_width + axisOffset, flip_y(pos) - text_height / 2, text_width, text_height};
    }

    // Render text
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);

    // Free surface and destroy texture
    SDL_FreeSurface(surface);

    SDL_DestroyTexture(texture);
}