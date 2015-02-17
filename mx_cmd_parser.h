#ifndef __MX_CMD_TOKEN_H__
#define __MX_CMD_TOKEN_H__
#include <stdio.h>
#include <stdbool.h>

#define MAX_PRECEDENCE_LEVEL 5
#define MAX_CMD_ARGS 5
/*
* The command token,
* Holds the command and the associated arguments.
*/
typedef struct cmd_token_t_ {
    // we can support 8 arguements for now.
    char *args[MAX_CMD_ARGS];
    // The processId of the forked process.
    int pid;
    // The pipe associated with the cmd.
    int pipefds[2];
} cmd_token_t;

extern bool parse_shell_input_cmd(char *inputcmdbuff, int cmdlen);
extern void parser_cleanup();
extern int parser_get_total_commands();
extern bool parser_pipe_init();
#endif 
