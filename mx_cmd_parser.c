#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mx_cmd_parser.h>
#include <mx_linkedlist.h>
#include <mx_debug.h>
#include <mx_utils.h>

#define MAX_PRECEDENCE_LEVEL 5

linked_list_t cmd_precedence_array[MAX_PRECEDENCE_LEVEL];

cmd_token_t* get_new_cmd_token () 
{
    return (malloc(sizeof(cmd_token_t)));
}

/*
void print_cmd_error (char* inputcmdbuff, int errorindex)
{
    int i = 0;
    printf("\n%s",inputcmdbuff);
    while (i <= errorindex) {
        printf(" ");
    }
    printf("^");
}
*/

void parser_cmd_token_free_func (void *data) 
{
    cmd_token_t *cmd_token = data;
    int args_index = 0;

    log("\nFree %s token", cmd_token->args[0]);
    while (args_index < MAX_CMD_ARGS) {
        if (cmd_token->args[args_index]) {
            free(cmd_token->args[args_index]);
        }
        args_index++;
    }

    free(cmd_token);
}

void parser_init_precedence_array ()
{
    int i = 0;
    while (i < MAX_PRECEDENCE_LEVEL) {
        linked_list_init(&cmd_precedence_array[i], parser_cmd_token_free_func);
        i++;
    }
}

bool parser_add_cmd_token_to_precedence_array
                        (linked_list_t *list,
                        cmd_token_t *cmd_token)
{
    return (linked_list_add_node(list, (void *)cmd_token));
}

cmd_token_t* parser_cmd_token_new () 
{
    cmd_token_t *cmd_token = NULL;
    cmd_token = malloc(sizeof(cmd_token_t));
    if (!cmd_token) {
        return NULL;
    }
    memset(cmd_token, 0, sizeof(cmd_token_t));

    return (cmd_token);
}

bool parse_shell_input_cmd (char* inputcmdbuff, int cmdlen)
{
    int curr_precedence_level = 5;
    char ch;
    int cmd_index = 0, cmd_arg_index = 0, curr_index;
    cmd_token_t *cmd_token = NULL;

    parser_init_precedence_array();

    while (cmd_index < cmdlen && 
           inputcmdbuff[cmd_index] != '\0') {
        ch = inputcmdbuff[cmd_index];
        cmd_index++;
        switch (ch) {
            case '(':
                // New precedence level.
                curr_precedence_level--;
                if (curr_precedence_level < 0) {
                    print_cmd_error(inputcmdbuff, cmd_index-1);
                    printf("\nOnly %d level of precedence is supported",
                            MAX_PRECEDENCE_LEVEL);
                    return false;
                }
                break;

            case ')':
                curr_precedence_level++;
                if (curr_precedence_level >= MAX_PRECEDENCE_LEVEL) {
                    print_cmd_error(inputcmdbuff, cmd_index-1);
                    printf("\nOnly %d level of precedence is supported",
                            MAX_PRECEDENCE_LEVEL);
                    return false;
                }
                break;

            case ',':
                // New command;
                if (!cmd_token) {
                    //  
                    print_cmd_error(inputcmdbuff, cmd_index-1);
                    printf("\nIllegal usage of \',\'");
                }
                if (parser_add_cmd_token_to_precedence_array(
                        &cmd_precedence_array[curr_precedence_level],
                        cmd_token)) {
                    return false;
                }
                cmd_token = NULL;
                break;
            case ' ':
                if (cmd_token) {
                    // We are in the middle of parsing a command, this should be
                    // the argument.
                    cmd_arg_index++;
                }


        }
    }
    return true;
}
