#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <mx-debug.h>
#include <mx-utils.h>

#define PROFILE_FILE "PROFILE"
#define MAX_LINE_BUF 80
#define MAX_CMD_BUF 1024


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
            setenv(env, filestr+valindex, 1);
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

bool parse_shell_input_cmd (char* inputcmdbuff, int cmdlen)
{
    return true;
}

int main()
{
    char cwdbuf[MAX_LINE_BUF];
    char *inputcmdbuff = NULL;
    size_t buffsize= MAX_CMD_BUF;
    size_t cmdlen;

    environment_init();

    inputcmdbuff = malloc(MAX_CMD_BUF);

    while (true) {
        printf("%s > ",getwd(cwdbuf));
        cmdlen = getline(&inputcmdbuff, &buffsize, stdin);
        
        inputcmdbuff[cmdlen-1]='\0';
        log("Shell input %s %d", inputcmdbuff, cmdlen);
        parse_shell_input_cmd(inputcmdbuff, cmdlen);
    }

    free(inputcmdbuff);
}
