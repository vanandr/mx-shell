#ifndef __SIGNALTEST_H__
#define __SIGNALTEST_H__ 
#include <signal.h>
void sigint_handler(int signum) {
  
     printf("Are you sure? [y/n] ");
     char c = getchar();
     if (c == 'y' || c == 'Y')
          exit(0);
     else
          signal(SIGINT, sigint_handler);
}

void sigalrm_disable(int signum) {
        // Disable sigalrm
}

#endif

