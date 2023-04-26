#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <stdbool.h>

#include "readMap.h"
#include "/opt/homebrew/include/proj.h" // #include <proj.h>
#include "/opt/homebrew/include/SDL2/SDL.h" //#include <SDL2/SDL.h>
#include "/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include/SDL2/SDL_ttf.h"

int find_node_index(Node *nodes, int node_count, int id);

int flip_y(int y);

void draw_map(Node *nodes, int num_nodes, Link *links, int num_links, Path *path);

void tickmarks_values(SDL_Renderer *renderer, TTF_Font *font, bool isXAxis, int index, int total_ticks, double minValue, double maxValue, double scale, double offset, int axisOffset);

#endif