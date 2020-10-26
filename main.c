#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE/2 + 1];
    char *hist_list[1000];
    int hist_index = 0;
    int should_run = 1;
    while (should_run) {
        printf("osh>");
        fflush(stdout);

        hist_list[hist_index++] = readCommand(args);
        // Read string from user and split into proper command and parameters.
        // Save the whole command to a history list.

        // Create child process
        pid_t child;
        int child_status;
        int check_wait = shouldWait(args);
        child = fork();
        switch(child) {
            case -1:
                printf("Fail to create a child process!.\n");
                exit(1);
                break;
            case 0:
                execvp(args[0], args);
                exit(0);
                break;
            default:
                if(check_wait)
                    wait(&child_status);
                // Co nen kiem tra va viet loi ra khong?
        }
        
    }
    return 0;
}