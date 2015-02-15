#ifndef __MX_CMD_TOKEN_H__
#include <stdio.h>

#define MAX_CMD_ARGS 5

typedef struct cmd_token_t_ {
    // we can support 8 arguements for now.
    char *args[MAX_CMD_ARGS];
    // The processId of the forked process.
    int pid;
    // The pipe associated with the cmd.
    int pipefds[2];
} cmd_token_t;

#endif 
