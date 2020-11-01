#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE/2 + 1];
    char *hist_list[1000];
    int hist_index = 0;
    int should_run = 1;
    printf("----Very Simple Shell\n\n");
    while (should_run) {
        printf("osh>");
        fflush(stdout);

        char *cmd = (char *)malloc(MAX_LINE * sizeof(char));
        fgets(cmd, MAX_LINE, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        // Replace '\n' with '\0'

        historySave(cmd, hist_list, &hist_index);
        // Save command to History

        int param_num = 0;
        splitCommand(cmd, args, &param_num);
        // Read string from user and split into seperated command and parameters.

        if(args[1] == NULL && (strcmp(args[0], "!!") == 0)) {
            loadLastCmd(hist_list, hist_index, args, param_num);
        }
        // Load the latest command into args
        // Should be placed above all external commands. Ex: history

        if(args[1] == NULL && (strcmp(args[0], "history") == 0)) {
            showHistory(hist_list, hist_index);
            continue;
        }

        int should_wait = 1;
        if(strcmp(args[param_num - 1], "&") == 0) {
            should_wait = 0;
            args[param_num - 1] = NULL;
            --param_num;
        }
        // Check whether or not the parent process is to wait for the child to exit.
        
        if(strcmp(cmd, "exit") == 0) {
            should_run = 0;
            continue;
        }

        // Create child process
        pid_t child;
        int child_status;
        
        child = fork();
        switch(child) {
            case -1:
                printf("Fail to create a child process!.\n");
                exit(EXIT_FAILURE);
                break;
            case 0:
                if(execvp(args[0], args) < 0) {
                    printf("Executing command fails.\n");
                }
                exit(EXIT_FAILURE);
                break;
            default:
                if(should_wait) {
                    wait(&child_status);
                }
                else {
                    printf("%ld\n", child);
                }
        }
        
    }
    return 0;
}