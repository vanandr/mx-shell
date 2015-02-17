#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <mx_debug.h>
#include <mx_utils.h>
#include <mx_cmd_parser.h>
#include <mx_calc_test.h>
#include "signaltest.h"
#include <signal.h>


#define PROFILE_FILE "PROFILE"
#define MAX_LINE_BUF 80
#define MAX_CMD_BUF 1024
extern void parser_close_clean_pipes();
extern bool parser_pipe_execute_cmds();

void environment_init ()
{
    size_t len = 0, read;
    char *filestr = NULL;
    FILE *profileFP;
    char env[MAX_LINE_BUF];

    profileFP = fopen(PROFILE_FILE, "r");

    if (!profileFP) {
        /* If the profile file doesn't exists return,
           Don't change any of the environment variable
         */
        return;
    }
    log("Reading profile file %s", PROFILE_FILE);

    while ((read = getline(&filestr, &len, profileFP)) != -1) {
        int envindex = 0, valindex, indexI;
        /* len is the amount of memory allocated and read is the amount memory
           read.
          */
        filestr[read-1]='\0';

        remove_white_spaces_until_character(filestr, 0, (int *)&read);

        while (envindex < read) {
            if (filestr[envindex] == ' ') {
                remove_white_spaces_until_character(filestr, envindex, 
                        (int *)&read);
                break;
            }
            envindex++;
        }

        if (filestr[envindex] != '=') {
            goto envloopcontinue;
        }

        if (envindex != 0 && envindex < read) {
            strncpy(env, filestr, envindex);
            env[envindex] = '\0';
        } else {
            goto envloopcontinue;
        }

        valindex = ++envindex;
        remove_white_spaces_until_character(filestr, valindex, (int *)&read);

        indexI = valindex;
        while (indexI < read) {
            if (filestr[indexI] == ' ') {
                remove_white_spaces_until_character(filestr, indexI, 
                                                    (int *)&read);
            }
            indexI++;
        }

        if (valindex != read) {
            setenv(env, filestr+valindex, 0);
            log("env var:%s:\"%s\"", env, getenv(env));
        } else {
            goto envloopcontinue;
        }

envloopcontinue:
        free(filestr);
        filestr = NULL;
        len = 0;
    }
    fclose(profileFP);
}

/**
  * Validate the braces and presence of other illegal characters
  */
bool parse_shell_validate_braces_chars (char *cmdbuff, int cmdlen)
{
    int bracecount = 0;
    int index = 0;

    while ((index < cmdlen) && cmdbuff[index] != '\0') {
        if (bracecount == 0 && cmdbuff[index] == 
                '0') {
            print_cmd_error(cmdbuff, index);
            printf("\nStart with \'(\' braces illegal use of braces\n");
            return false;
        }

        if (cmdbuff[index] == '[' ||
            cmdbuff[index] == ']' ||
            cmdbuff[index] == '}' ||
            cmdbuff[index] == '{' ||
            cmdbuff[index] == '|' ||
            cmdbuff[index] == '&') {
            print_cmd_error(cmdbuff, index);
            printf("\nIllegal character %c encountered\n", cmdbuff[index]);
            return false;
        }

        if (cmdbuff[index] == '(') {
            bracecount++;
        }

        if (cmdbuff[index] == ')') {
            bracecount--;
        }
        index++;
    }

    if (bracecount != 0) {
        print_cmd_error(cmdbuff, index);
        printf("\nMismatching braces in the command\n");
        return false;
    }

    return true;

}

bool parse_shell_validate_cmd (char *cmdbuff, int cmdlen)
{
    if (!parse_shell_validate_braces_chars(cmdbuff, cmdlen)) {
        log("Failed to validate braces");
        return false;
    }
    return true;
}

int main()
{
    char cwdbuf[MAX_LINE_BUF];
    char *inputcmdbuff = NULL;
    size_t buffsize= MAX_CMD_BUF;
    size_t cmdlen;

    system("clear");
    signal(SIGINT, sigint_handler);

    environment_init();
    chdir(getenv("HOME"));

    printf(KYEL"\n !!!!!!!   Welcome to the MX-SHELL  !!!!!!\n");
    printf("\n Source code available @ https://github.com/vanandr/mx-shell\n");
    printf("\n Environmental variables are parsed from %s file\n",PROFILE_FILE);
    printf("\n Command help:");
    printf("\n clear - Clear screen");
    printf("\n exit - Exists the MX-Shell");
    printf("\n calc - Starts the Calculator Program");
    printf("\n (cmd1, (cmd2), cmd3) - Pipelines cmds in the following order");
    printf ("\n \t\t\tcmd2-->cmd1-->cmd3\n"KNRM);


    inputcmdbuff = malloc(MAX_CMD_BUF);

    while (true) {
        printf(KCYN"%s > "KNRM,getwd(cwdbuf));
        cmdlen = getline(&inputcmdbuff, &buffsize, stdin);

        inputcmdbuff[cmdlen-1]='\0';

        if (!parse_shell_validate_cmd(inputcmdbuff, cmdlen)) {
            goto parser_cleanup;
        }

        if (!strncmp(inputcmdbuff,"calc",4)) {
            calctest();
            continue;
        }

        if (!strncmp(inputcmdbuff, "exit", 4)) {
            exit(0);
        }

        if (!strncmp(inputcmdbuff, "clear", 4)) {
            system("clear");
            continue;
        }

        if (!parse_shell_input_cmd(inputcmdbuff, cmdlen)) {
            goto parser_cleanup;
        }

        if (!parser_pipe_init()) {
            goto parser_cleanup;
        }

        if (!parser_pipe_execute_cmds()) {
            goto parser_cleanup;
        }
parser_cleanup:
        parser_close_clean_pipes();
        parser_cleanup();

        log("Shell input %s %d", inputcmdbuff, cmdlen);
    }

    free(inputcmdbuff);
}
