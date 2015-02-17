#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mx_cmd_parser.h>
#include <mx_linkedlist.h>
#include <mx_debug.h>
#include <mx_utils.h>
#include <unistd.h>

#define MAX_PRECEDENCE_LEVEL 5

linked_list_t cmd_precedence_array[MAX_PRECEDENCE_LEVEL];
void 
parser_print_cmd_token (void *data)
{
    int index = 0;
    cmd_token_t *cmd_token = data;

    printf("{");
    while (index < MAX_CMD_ARGS) {
        if (cmd_token->args[index]) {
            printf("[%d]%s,",index, cmd_token->args[index]);
        }
        index++;
    }
   printf("RFD:%d,WFD:%d",cmd_token->pipefds[0], cmd_token->pipefds[1]);
   printf("} ");
}
void
print_cmd_precedence_array ()
{
    int index = 0;
    linked_list_t *linked_list = NULL;

    while (index < MAX_PRECEDENCE_LEVEL) {
        linked_list = &cmd_precedence_array[index];
        printf("\nPrecedence %d: total commands %d:", index,
                linked_list_get_node_count(linked_list));
        linked_list_walk(linked_list, parser_print_cmd_token);
        index++;
    }
    printf("\n");
}

void parser_cmd_token_free_func (void *data) 
{
    cmd_token_t *cmd_token = data;
    int args_index = 0;

    log("Free %s token", cmd_token->args[0]);
    while (args_index < MAX_CMD_ARGS) {
        if (cmd_token->args[args_index]) {
            free(cmd_token->args[args_index]);
        }
        args_index++;
    }

    free(cmd_token);
}

int parser_get_total_commands () 
{
    int i = 0, cnt = 0;
    while (i < MAX_PRECEDENCE_LEVEL) {
        cnt += linked_list_get_node_count(&cmd_precedence_array[i]);
        i++;
    }
    return cnt;
}

void parser_init_precedence_array ()
{
    int i = 0;
    while (i < MAX_PRECEDENCE_LEVEL) {
        linked_list_init(&cmd_precedence_array[i], parser_cmd_token_free_func);
        i++;
    }
}

void parser_cleanup ()
{
    int i = 0;
    while (i < MAX_PRECEDENCE_LEVEL) {
        linked_list_destroy(&cmd_precedence_array[i]);
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
    cmd_token->pipefds[0] = -1;
    cmd_token->pipefds[1] = -1;

    return (cmd_token);
}

bool parse_shell_input_cmd (char* inputcmdbuff, int cmdlen)
{
    char ch;
    int curr_precedence_level = MAX_PRECEDENCE_LEVEL;
    int cmd_index = 0, cmd_arg_index = 0, curr_index = 0;
    int sub_cmd_start_index = 0;
    cmd_token_t *cmd_token = NULL;
    int cmd_param_start_index = 0;

    int cnt = 0;

    parser_init_precedence_array();

    while (cmd_index < cmdlen -1) {
        ch = inputcmdbuff[cmd_index];

        switch (ch) {
            case '(':
                // New precedence level.
                curr_precedence_level--;
                if (curr_precedence_level < 0) {
                    print_cmd_error(inputcmdbuff, cmd_index);
                    printf("\nOnly %d level of precedence is supported\n",
                            MAX_PRECEDENCE_LEVEL);
                    goto parser_cleanup;
                }
                if (cmd_token) {
                    // we are still parsing the command.
                    print_cmd_error(inputcmdbuff, cmd_index-1);
                    printf("\nEvery command should end with a \',\'\n");
                    goto parser_cleanup;
                }
                break;

            case ')':
                if (cmd_token &&
                    !parser_add_cmd_token_to_precedence_array(
                        &cmd_precedence_array[curr_precedence_level],
                        cmd_token)) {
                    goto parser_cleanup;
                }
                cmd_token = NULL;
                curr_precedence_level++;
                if (curr_precedence_level > MAX_PRECEDENCE_LEVEL) {
                    print_cmd_error(inputcmdbuff, cmd_index);
                    printf("\nOnly %d level of precedence is supported\n",
                            MAX_PRECEDENCE_LEVEL);
                    goto parser_cleanup;
                }
                if ((curr_precedence_level == MAX_PRECEDENCE_LEVEL) &&
                        cmd_index !=(cmdlen-2)) {
                    // We have some more cmds to parse but we have reached EOB
                    print_cmd_error(inputcmdbuff, cmd_index);
                    printf("\nMismatched braces\n");
                    goto parser_cleanup;
                }

                break;

            case ',':
                // New command;
                if (cmd_token && !parser_add_cmd_token_to_precedence_array(
                        &cmd_precedence_array[curr_precedence_level],
                        cmd_token)) {
                    goto parser_cleanup;
                }
                cmd_token = NULL;
                break;
            case ' ':
                break;

            default:
                cmd_param_start_index = cmd_index;
                if (!cmd_token) {
                    // this is start of a new token.
                    cmd_token = parser_cmd_token_new();
                    cmd_arg_index = 0;
                } else {
                    cmd_arg_index++;
                }
                while (cmd_index < cmdlen) {
                    ch = inputcmdbuff[cmd_index];
                    if (ch == ' ' || ch == ',' || ch == '(' || ch == ')') {
                        cmd_index--;
                        break;
                    }
                    cmd_index++;
                }
                if (cmd_index == cmdlen) {
                    print_cmd_error(inputcmdbuff, cmd_param_start_index);
                    printf("\nIllegal Command provided");
                    goto parser_cleanup;
                }
                cmd_token->args[cmd_arg_index] = 
                    strndup(inputcmdbuff+ cmd_param_start_index,
                            cmd_index-cmd_param_start_index+1);
        }
        cmd_index++;
    }

#ifdef DEBUG 
//    print_cmd_precedence_array();
#endif
    return true;
parser_cleanup:
    return false;
}
