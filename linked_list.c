#include <stdio.h>
#include <linked-list.h>
#include <stdbool.h>

bool linked_list_init (linked_list_t *list) 
{
    if (!list) {
        return false;
    }
    memset(list, 0, sizeof(linked_list_t));
    return true;
}

bool linked_list_add_node (linked_list_t *list, 
                                list_node_t *node)
{
    list_node_t *curr_node;
    if (!list) {
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
    return true;
}
