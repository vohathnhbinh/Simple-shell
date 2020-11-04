#include "header.h"

int main(void) {
    
    char *args[MAX_LINE/2 + 1];
    char *hist_list[1000];
    int hist_num = 0;
    int should_run = 1;
    printf("\n----Very Simple Shell\n\n");
    while (should_run) {
        for(int i = 0; i < MAX_LINE/2 + 1; i++) {
            args[i] = NULL;
        }
        // Empty args

        printf("osh>");
        fflush(stdout);

        char *cmd = (char *)malloc((MAX_LINE + 1) * sizeof(char));
        fgets(cmd, MAX_LINE + 1, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        // Replace '\n' with '\0'

        int isEmpty = 0;
        if(cmd[0] == '\0') {
            isEmpty = 1;
        }

        int param_num = 0;
        int shouldShowHistory = 0;
        int should_wait = 1;
        if(!isEmpty) {
            historySave(cmd, hist_list, &hist_num);
            // Save command to History

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
                cmd[strlen(cmd) - 1] = '\0';
            }
            // Check whether or not the parent process is to wait for the child to exit.
            
            if(strcmp(cmd, "exit") == 0) {
                should_run = 0;
                continue;
            }
        }
        printf("num: %d\n", param_num);
        // Create child process
        pid_t child;
        int child_status;
        child = fork();
        int direction = isDirection(args, param_num);
        switch(child) {
            case -1:
                printf("Fail to create a child process!\n");
                exit(EXIT_FAILURE);
                break;
            case 0:
                if(shouldShowHistory) {
                    showHistory(hist_list, hist_num);
                } else if (direction == 1) {
					int fd; 
					char* filename = (char*)malloc(sizeof(char) * strlen(args[param_num-1]));					
					mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;	
					
					strcpy(filename,args[param_num-1]);
					printf("Redirect > %s\n", filename);
					fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode); 
					if (fd < 0)
						{
							perror("open error");
							exit(0);
						}
					
					dup2(fd, STDOUT_FILENO);
					
					execvp(args[0],args);
					close(fd);
				} else if (direction == 2) {
					int fd;
					char* filename = (char*)malloc(sizeof(char) * strlen(args[param_num-1]));						
					strcpy(filename,args[param_num-1]);
					printf("Redirect < %s\n", filename);
					fd = open(filename, O_RDONLY, 0); 
					if (fd < 0)
						{
							perror("open error");
							exit(0);
						}
					
					dup2(fd, STDIN_FILENO);

					execvp(args[0],args);
					close(fd);
				}
                else if(execvp(args[0], args) < 0) {
                    printf("%s: Command not found.\n", args[0]);
                }
                exit(EXIT_FAILURE);
                break;
            default:
                if(should_wait) {
                    wait(&child_status);
                }
                else {
                    printf("%ld\n", child);
                    waitpid(child, &child_status, WUNTRACED);
                    if(WIFEXITED(child_status)) {
                        printf("+ Exit\t\t\t%s\n", cmd);
                    }
                }
        }
        
    }
    return 0;
}