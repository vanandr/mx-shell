#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <mx_linkedlist.h>


bool linked_list_init (linked_list_t *list, list_free_func_t free_func) 
{
    if (!list) {
        return false;
    }
    memset(list, 0, sizeof(linked_list_t));
    list->free_func = free_func;
    return true;
}

bool linked_list_add_node (linked_list_t *list, 
                           void *data)
{
    list_node_t *curr_node, *node;
    if (!list) {
        return false;
    }

    node = malloc(sizeof(list_node_t));
    memset(node, 0, sizeof(list_node_t));

    node->next = NULL;
    node->data = data;

    if (!node) {
        return false;
    }

    if (list->head) {
        if (!list->recent_node) {
            return false;
        }

        curr_node = list->recent_node;
        curr_node->next = node;
    } else {
        list->head = node;
    }
    node->next = NULL;
    list->recent_node = node;
    list->node_cnt++;
    return true;
}

int linked_list_get_node_count (linked_list_t *list)
{
    int cnt = 0;
    if (list) {
        cnt = list->node_cnt;
    }
    return cnt;
}

bool linked_list_destroy (linked_list_t *list)
{
    list_node_t *node = NULL, *tmp_node = NULL;

    if (!list) {
        return false;
    }

    node = list->head;
    while (node) {
        list->free_func(node->data);
        tmp_node = node->next;
        free(node);
        node = tmp_node;
    }

    linked_list_init(list, NULL);
    return true;
}

bool linked_list_walk (linked_list_t *list, 
                       list_walk_func_t walk_func)
{
    list_node_t *node = NULL, *tmp_node = NULL;

    if (!list) {
        return false;
    }

    node = list->head;
    while (node) {
        walk_func(node->data);
        node = node->next;
    }
    return true;
}
