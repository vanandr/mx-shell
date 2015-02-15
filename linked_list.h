#ifndef __LINKED-LIST_H__
#include <stdio.h>
#include <stdbool.h>

typdef struct list_node_t_ {
    struct list_node_t_ *next;
    void *data;
} list_node_t;

typedef struct linked_list_t_ {
    int node_cnt;
    list_node_t *head;
    list_node_t *recent_node;
} linked_list_t;

extern bool linked_list_init(linked_list_t *list);
extern bool linked_list_add_node(linked_list_t *list,list_node_t *node);
