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

#define CMD_CNT 3

int main ()
{
    cmd_token_t cmd[CMD_CNT];
    int pipesfd[2];
    int index = 0;
    int status;
    
    printf(" Main process pid %d", getpid());

    cmd[0].args[0] = "ls";
    cmd[0].args[1] = "-l";
    cmd[0].args[2] = NULL;
    cmd[0].pipefds[0] = -1;
    cmd[0].pipefds[1] = -1;

    cmd[1].args[0] = "sort";
    cmd[1].args[1] = NULL;
    cmd[1].pipefds[0] = -1;
    cmd[1].pipefds[1] = -1;

    cmd[2].args[0] = "wc";
    cmd[2].args[1] = "-l";
    cmd[2].args[2] = NULL;
    cmd[2].pipefds[0] = -1;
    cmd[2].pipefds[1] = -1;

    int pipefd[CMD_CNT-1][2];
    
    while (index < CMD_CNT-1) {
        if (pipe(&pipefd[index][0]) < 0) {
            printf("\n Failed piping");
        }

        cmd[index].pipefds[PIPE_WRITE_FD] = pipefd[index][PIPE_WRITE_FD];
        cmd[index+1].pipefds[PIPE_READ_FD] = pipefd[index][PIPE_READ_FD];
        index++;
    }
    int pipetempfd[2];
    pipe(&pipetempfd[0]);

    printf("\n temp fd pipes read %d, write %d", pipetempfd[PIPE_READ_FD],
            pipetempfd[PIPE_WRITE_FD]);
   
    index = 0;
    int cnt = 0;

    while (index < CMD_CNT) {
        int pid =0, status;

        printf("\npid:%d forking process index %d \n", 
                getpid(), index );
        pid = fork();

        if (pid == 0) {

            // child process execute command.

            if (cmd[index].pipefds[PIPE_READ_FD] >= 0) {
                close(STDIN_FILENO);
                dup(cmd[index].pipefds[PIPE_READ_FD]);
            }


            if (cmd[index].pipefds[PIPE_WRITE_FD] != -1)  {
                close(STDOUT_FILENO);
                dup(cmd[index].pipefds[PIPE_WRITE_FD]);
            }

            int i = 0;
            while (i < CMD_CNT) {
                if (cmd[i].pipefds[PIPE_READ_FD] >= 0) {
                    close(cmd[i].pipefds[PIPE_READ_FD]);
                }
                if (cmd[i].pipefds[PIPE_WRITE_FD] >= 0) {
                    close(cmd[i].pipefds[PIPE_WRITE_FD]);
                }
                i++;
            }

            if (execvp(cmd[index].args[0], cmd[index].args) < 0) {
                printf("\n Error executing command");
            }
            
        } else {
            cmd[index].pid = pid;
            index++;
            printf("\n\n\npid:%d in parent process child pid %d",getpid(), pid);
            waitpid(pid, &status, 0);
        }
    }
}
