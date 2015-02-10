#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <debug.h>
#include <string.h>

#define PROFILE_FILE "PROFILE"
#define MAX_LINE_BUF 80

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
        int envindex = 0, valindex;
        /* len is the amount of memory allocated and read is the amount memory
           read.
          */
        filestr[read-1]='\0';

        while (envindex < read) {
            if (filestr[envindex] == '=' || 
                filestr[envindex] == ' ') {
                break;
            }
            envindex++;
        }

        if (envindex != 0 && envindex < MAX_LINE_BUF) {
            strncpy(env, filestr, envindex);
            env[envindex] = '\0';
        } else {
            goto envloopcontinue;
        }

        valindex = ++envindex;
        while (valindex < read) {
            if (filestr[valindex] != ' ' &&
                filestr[valindex] != '\0') {
                break;
            }
            valindex++;
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
}



int main()
{
    char cwdbuf[MAX_LINE_BUF];
    char *inputcmd = NULL;
    size_t buffsize= MAX_LINE_BUF;
    size_t linelen;

    environment_init();

    inputcmd = malloc(MAX_LINE_BUF);

    while (true) {
        printf("%s > ",getwd(cwdbuf));
        linelen = getline(&inputcmd, &buffsize, stdin);
        
        inputcmd[linelen-1]='\0';
        log("Shell input %s %d", inputcmd, linelen);
    }
}
