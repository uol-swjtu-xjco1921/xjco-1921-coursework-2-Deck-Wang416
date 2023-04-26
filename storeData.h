#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/opt/homebrew/include/proj.h" // #include <proj.h>

typedef struct Node
{
    int id;

    double lat; //纬度
    
    double lon; //经度
} Node;

typedef struct Link
{
    int id; // 连接的唯一标识符

    int node1;

    int node2;

    double length;

    double veg;

    double arch;

    double land;
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
} ListType; //枚举类型，表示链表的类型

typedef struct DataLists
{
    NodeList *nodeList;

    LinkList *linkList;

    int node_count;

    int link_count;
} DataLists; //包含了节点链表和连接链表以及它们的计数

typedef struct 
{
    int length; //路径中的节点数量

    Node *path;
} Path;//从起点到终点的路径

void free_list(void *list, ListType type);

DataLists store_data(const char *filename);

#endif