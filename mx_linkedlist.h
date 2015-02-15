#ifndef __MX_LINKED_LIST_H__
#define __MX_LINKED_LIST_H__
#include <stdio.h>
#include <stdbool.h>

typedef void (*list_free_func_t) (void *data);
typedef void (*list_walk_func_t) (void *data);

typedef struct list_node_t_ {
    struct list_node_t_ *next;
    void *data;
} list_node_t;

typedef struct linked_list_t_ {
    int node_cnt;
    list_free_func_t free_func;
    list_node_t *head;
    list_node_t *recent_node;
} linked_list_t;

extern bool linked_list_init(linked_list_t *list, list_free_func_t free_func);
extern bool linked_list_add_node(linked_list_t *list,void *data);
extern bool linked_list_walk(linked_list_t *list, list_walk_func_t walk_func);
extern int linked_list_get_node_count(linked_list_t *list);
extern bool linked_list_destroy(linked_list_t *list);

#endif //__LINKED-LIST_H__
