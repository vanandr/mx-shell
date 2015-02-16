#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mx_cmd_parser.h>
#include <mx_linkedlist.h>
#include <mx_debug.h>
#include <string.h>

#define PIPE_READ_FD STDIN_FILENO
#define PIPE_WRITE_FD STDOUT_FILENO

int **g_pipefds = NULL;
int g_total_cmds = 0;

extern linked_list_t cmd_precedence_array[MAX_PRECEDENCE_LEVEL];
extern void print_cmd_precedence_array();

void close_clean_pipes ()
{
    int index = 0;

    while (index < (g_total_cmds-1)) {
        if(g_pipefds[index][PIPE_READ_FD] != -1) {
            close(g_pipefds[index][PIPE_READ_FD]);
        }

        if(g_pipefds[index][PIPE_WRITE_FD] != -1) {
            close(g_pipefds[index][PIPE_WRITE_FD]);
        }
        index++;
    }
    free(g_pipefds);
}

bool parser_pipe_init () 
{
    int index = 0;
    int precedence_index = 0;
    list_node_t *node = NULL;
    list_node_t *next_node = NULL; 
    cmd_token_t *curr_token = NULL, *next_token = NULL;
    linked_list_t *list = NULL;

    g_total_cmds = parser_get_total_commands();

    if (!g_total_cmds) {
        log("No commands enetered");
        return true;
    }

    g_pipefds = (int **)malloc((g_total_cmds - 1) * 2 * sizeof(int));
    if (!g_pipefds) {
        log("Failed to allocate memory for pipe");
        return false;
    }
    
    memset(g_pipefds, -1, (g_total_cmds - 1) * 2 * sizeof(int)); 

    while (index < (g_total_cmds - 1)) {
        if (!pipe(&g_pipefds[index][0]) < 0) {
            log("Failed to initialize pipes");
            close_clean_pipes();
            return false;
        }
        index++;
    }

    precedence_index = 0;
    index = 0;
    node = NULL;

    while (node == NULL &&
           precedence_index < MAX_PRECEDENCE_LEVEL) {
        list = &cmd_precedence_array[precedence_index];
        node = list->head;
        log("precedence index %d node %p", precedence_index, node);
        precedence_index++;
    }

    while (node) {
        next_node = node->next;
        while (next_node == NULL &&
               precedence_index < MAX_PRECEDENCE_LEVEL) {
            list = &cmd_precedence_array[precedence_index];
            next_node = list->head;
            precedence_index++;
        }
        if (next_node) {
            if (index >= (g_total_cmds - 1)) {
                log("Mismatch in the total commands and the allocated FDs");
                close_clean_pipes();
                return false;
            }
            curr_token = (cmd_token_t *)node->data;
            next_token = next_node->data;
            curr_token->pipefds[PIPE_WRITE_FD] = 
                g_pipefds[index][PIPE_WRITE_FD];
            next_token->pipefds[PIPE_READ_FD] = 
                g_pipefds[index][PIPE_READ_FD];
            index++;
        }
        node = next_node;
    }
#ifdef DEBUG
    print_cmd_precedence_array();
#endif

    return true;
}
