#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PROFILE_FILE "./PROFILE"
int main () 
{
    char *args[3];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = NULL;
    char ch;
    char *pathvar = getenv("PATH");
    size_t len = 0, read;
 //   printf("path = %s",pathvar);

    char *filestr = NULL;

    FILE *profileFP;

    profileFP = fopen(PROFILE_FILE, "r");

    if (!profileFP) {
        printf("File doesn exists");
        return 1;
    }

    printf("\n printing from file");
    while ((read = getline(&filestr, &len, profileFP)) != -1) {
        filestr = NULL;
        len = 0;
    }
    

    //printf("\n printing done from file");
   // setenv("PATH", "/root/shellprogram/mx-shell:/bin", 1);

//    printf("\n befre execvp");
//    pathvar = getenv("PATH");
//    printf("path = %s",pathvar);
    //execvp(args[0], args);
}

