#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exit codes 
#define EXIT_WITHOUT_ERRORS 0
#define EXIT_WITH_ERRORS -1

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

typedef struct DataLists
{
    NodeList *nodeList;

    LinkList *linkList;

    int node_count;

    int link_count;
} DataLists;

typedef struct 
{
    int length;

    Node *path;
} Path;

void free_list(void *list, ListType type);

DataLists store_data(const char *filename);

#endif