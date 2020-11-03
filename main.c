#include "header.h"

#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE/2 + 1];
    char *hist_list[1000];
    int hist_num = 0;
    int should_run = 1;
    int check_child1 = 0, check_child2 = 0;
    pid_t previous_pid;
    pid_t previousprevious_pid;
    printf("----Very Simple Shell\n\n");
    while (should_run) {
        for(int i = 0; i < MAX_LINE/2 + 1; i++) {
            args[i] = NULL;
        }
        // Empty args

        signal(SIGCHLD, childHandler);

        printf("osh>");
        fflush(stdout);

        char *cmd = (char *)malloc(MAX_LINE * sizeof(char));
        fgets(cmd, MAX_LINE, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        // Replace '\n' with '\0'

        int isEmpty = 0;
        if(cmd[0] == '\0') {
            isEmpty = 1;
        }

        int shouldShowHistory = 0;
        int should_wait = 1;
        if(!isEmpty) {
            historySave(cmd, hist_list, &hist_num);
            // Save command to History

            int param_num = 0;
            splitCommand(cmd, args, &param_num);
            // Read string from user and split into seperated command and parameters

            if(strcmp(cmd, "!!") == 0) {
                if(!loadLastCmd(hist_list, hist_num, args, &param_num)) {
                    continue;
                }
            }
            // Load the latest command into args
            // Should be placed above all external commands. Ex: history

            if(cmd[0] == '!' && cmd[1] != '!') {
                if(!loadSpecificCmd(hist_list, cmd, hist_num, args, &param_num)) {
                    continue;
                }
            }
            // Load a specific command by entry
            // Maximum entry is 1000

            if(strcmp(cmd, "history &") == 0 ||
            (strcmp(args[0], "history") == 0 && args[1] == NULL)) {
                shouldShowHistory = 1;
            }
           
            if(strcmp(args[param_num - 1], "&") == 0 && strcmp(args[0], "&") != 0) {
                should_wait = 0;
                args[param_num - 1] = NULL;
                --param_num;
            }
            // Check whether or not the parent process is to wait for the child to exit.
            
            if(strcmp(cmd, "exit") == 0) {
                should_run = 0;
                continue;
            }
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
                if(shouldShowHistory) {
                    showHistory(hist_list, hist_num);
                }
                else if(execvp(args[0], args) < 0) {
                    printf("Executing command fails.\n");
                }
                exit(EXIT_FAILURE);
                break;
            default:
                if(should_wait) {
                    wait(&child_status);
                    if(check_child1) {
                        printf("Child %ld has exited.\n", previous_pid);
                        check_child1 = 0;
                    }
                }
                else {
                    printf("Child %ld\n", child);
                    previous_pid = child;
                    check_child1 = 1;
                    if(check_child2) {
                        printf("Child %ld has exited.\n", previousprevious_pid);
                        check_child2 = 0;
                    }
                    check_child2 = 1;
                    previousprevious_pid = previous_pid;
                }
        }
        
    }
    return 0;
}