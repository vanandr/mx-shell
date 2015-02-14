#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define PIPE_READ_FD STDIN_FILENO
#define PIPE_WRITE_FD STDOUT_FILENO

typedef struct cmd_token_t_ {
    char *args[10];
    int arglen;
    int pid;
    int pipefds[2];
} cmd_token_t;

#define CMD_CNT 2

int main ()
{
    cmd_token_t cmd[2];
    int pipesfd[2];
    int index = 0;
    
    printf(" Main process pid %d", getpid());

    cmd[0].args[0] = "ls";
    cmd[0].args[1] = "-l";
    cmd[0].args[2] = NULL;
    cmd[0].pipefds[0] = -1;
    cmd[0].pipefds[1] = -1;

    cmd[1].args[0] = "wc";
    cmd[1].args[1] = "-l";
    cmd[1].args[2] = NULL;
    cmd[1].pipefds[0] = -1;
    cmd[1].pipefds[1] = -1;

    int fd[2];
    pipe(&fd[0]);

    printf(" read fd %d, write fd %d\n", fd[0], fd[1]);

    index = 0;
    while (index < 2) {
        if (fork() != 0) {
            if (index == 0) {
                printf("\n in parent process \n");
                close(fd[0]);
                close(STDOUT_FILENO);
                dup(fd[1]);
                close(fd[1]);
                printf("\n parent process write fd %d\n", fd[1]);
                execvp(cmd[0].args[0], cmd[0].args);
            }
            if (index == 1) {
                close(fd[1]);
                close(STDIN_FILENO);
                dup(fd[0]);
                close(fd[0]);
                printf("\n chile process read fd %d\n", fd[0]);
                execvp(cmd[1].args[0], cmd[1].args);
            }
        } else {
            index ++;
        }
    }
    /*else {
        printf("\n in child process \n");
        close(fd[1]);
        close(STDIN_FILENO);
        dup(fd[0]);
        close(fd[0]);
        printf("\n chile process read fd %d\n", fd[0]);
        execvp(cmd[1].args[0], cmd[1].args);
    }
    */
}
