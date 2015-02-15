#include <mx_cmd_token.h>

#define MAX_PRECEDENCE_LEVEL 5

linked_list_t* cmd_precedence_array[MAX_PRECEDENCE_LEVEL];

cmd_token_t* get_new_cmd_token () 
{
    return (malloc(sizeof(cmd_token_t)));
}

void print_cmd_error (char* inputcmdbuff, int currindex)
{
    int i = 0;
    printf("\n%s",inputcmdbuff);
    while (i <= currindex) {
        printf(" ");
    }
    printf("^");
}

void parser_init_precedence_array ()
{
    int i = 0;
    while (i < MAX_PRECEDENCE_LEVEL) {
        cmd_precedence_array[i] = NULL;
        i++;
    }
}

bool parse_shell_input_cmd (char* inputcmdbuff, int cmdlen,
                            linked_list_t *_cmd_precedence_array[],
                            int *max_precedence_level)
{
    int curr_precedence_level = 5;
    char ch;
    int cmd_index = 0;
    cmd_token_t *cmd_token = NULL;
    *max_precedence_level = MAX_PRECEDENCE_LEVEL;

    parser_init_presedence_array(cmd_precedence_array);

    while (cmd_index < cmdlen && 
           inputcmdbuff[cmd_index] != '\0') {
        ch = inputcmdbuff[cmd_index];
        cmd_index++;
        switch (ch) {
            case '(':
                // New precedence level.
                curr_precedence_level--;
                if (curr_precedence_level < 0) {
                    print_cmd_error(inputcmdbuff, inputcmdbuff);
                    printf("\nOnly %d level of precedence is supported",
                            MAX_PRECEDENCE_LEVEL);
                    return false;
                }
                break;

            case ')':
                curr_precedence_level++;
                if (curr_precedence_level >= MAX_PRECEDENCE_LEVEL) {
                    print_cmd_error(inputcmdbuff, inputcmdbuff);
                    printf("\nOnly %d level of precedence is supported",
                            MAX_PRECEDENCE_LEVEL);
                    return false;
                }
                break;

            case ',':
                // New command;
                if (!cmd_token) {
                    //  
                    print_cmd_error(inputcmdbuff, cmd_index);
                    printf("\nIllegal usage of \',\'");
                }
                parser_add_cmd_token(cmd_token,
        }
    }
}
