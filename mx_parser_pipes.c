#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mx_cmd_parser.h>
#include <mx_linkedlist.h>
#include <mx_debug.h>

#define PIPE_READ_FD STDIN_FILENO
#define PIPE_WRITE_FD STDOUT_FILENO

int **pipefds;

extern linked_list_t cmd_precedence_array[MAX_PRECEDENCE_LEVEL];

bool parser_pipe_init () 
{
    int total_cmds = parser_get_total_commands();
    int index = 0;
    pipefds = (int **)malloc((total_cmds - 1) * 2 * sizeof(int));
    int precedence_index = 0;

    if (!pipefds) {
        return false;
    }

    while (index < (total_cmds - 1)) {
        if (!pipe(&pipefds[index][0]) < 0) {
            log("Failed to initialize pipes");
//            close_clean_pipes(pipefds, total_cmds-1);
            pipefds = NULL;
            return false;
        }
        index++;
    }

    index = 0;
    while (index < (total_cmds - 1)) {
        if (precedence_index >= MAX_PRECEDENCE_LEVEL) {
        }
    }

    return true;
}
