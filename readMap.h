#ifndef READMAP_H
#define READMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

// Exit codes 
#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_FILE_NAME 2
#define EXIT_MALLOC_FAILED 3
#define EXIT_SDL_ERRORS 4
#define EXIT_BAD_ATTRIBUTES 5
#define EXIT_BAD_LABELS 6

typedef struct Bounding
{
    double minLat;

    double minLon;

    double maxLat;

    double maxLon;
} Bounding;

typedef struct Node 
{
    int id;

    double lat;
    
    double lon;
} Node;

typedef struct Link
{
    int id;

    int node1;

    int node2;

    int way;

    double length;

    double veg;

    double arch;

    double land;

    double speed; 

    int *POI;

    int poi_count;
} Link;

typedef struct LinkList
{
    Link data;

    struct LinkList *next;
} LinkList;

typedef struct NodeList
{
    Node data;

    struct NodeList *next;
} NodeList;

typedef enum 
{
    LINK_LIST,

    NODE_LIST,
} ListType;

typedef struct UnparsedTag
{
    char *line;
    struct UnparsedTag *next;
} UnparsedTag;

typedef struct DataLists
{
    NodeList *nodeList;

    LinkList *linkList;

    Bounding bounding;

    UnparsedTag *unparsedTags; 

    int node_count;

    int link_count;

    int unparsed_count;
} DataLists;

typedef struct 
{
    int length;

    Node *path;

    double total_length;
} Path;

bool is_bounding_format_correct(const char* line);

bool is_link_format_correct(const char* line);

bool is_node_format_correct(const char* line);

void free_list(void *list, ListType type);

void free_unparsed_tags(UnparsedTag *list);

DataLists parse_and_store_data(const char *filename);

#endif