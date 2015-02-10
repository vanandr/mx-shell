#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>

void environment_init ()
{
    size_t len = 0, read;

    profileFP = fopen(PROFILE_FILE, "r");

    if (!profileFP) {
        /* If the profile file doesn't exists return,
           Don't change any of the environment variable
         */
        return;
    }

    while ((read = getline(&filestr, &len, profileFP)) != -1) {
        /* len is the amount of memory allocated and read is the amount memory
           read.
          */
        
        free(filestr);
        filestr = NULL;
        len = 0;
    }
}


int main()
{
    char cwdbuf[80];
    char *inputcmd = NULL;
    size_t buffsize=0;
    size_t linelen;

    environment_init();

    while (true) {
        printf("%s > ",getwd(cwdbuf));
        linelen = getline(&inputcmd, &buffsize, stdin);
        
        free (inputcmd);
    }
}
