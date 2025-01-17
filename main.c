#include "header.h"

int main(void) {
    char *args[MAX_LINE/2 + 1];
    char *right[MAX_LINE/2 + 1];
    char *hist_list[1000];
    int hist_num = 0;
    int should_run = 1;
    printf("\n----Welcome to the True Man's world\n\n");
    while (should_run) {
        printf("\033[1;31m"); // Change text color to red for screenshotting testcases
        printf("lulu>");
        printf("\033[0m"); // Reset text color
        fflush(stdout);

        char *cmd = (char *)malloc((MAX_LINE + 1) * sizeof(char));
        fgets(cmd, MAX_LINE + 1, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        // Replace '\n' with '\0'

        int is_empty = 0;
        if(cmd[0] == '\0') {
            is_empty = 1;
        }

        int param_num_l = 0;
        int param_num_r = 0;
        int should_show_history = 0;
        int should_wait = 1;
        int is_pipe = 0;
        if(!is_empty) {
            historySave(cmd, hist_list, &hist_num);
            // Save command to History

            is_pipe = tokenizeCmd(cmd, args, right, &param_num_l, &param_num_r);
            // Read string from user and split into seperated command and parameters

            if(strcmp(cmd, "!!") == 0) {
                if(!loadLastCmd(hist_list, hist_num, args, right, &param_num_l, &param_num_r, &is_pipe)) {
                    continue;
                }
            }
            // Load the latest command into args
            // Should be placed above all builtin commands. Ex: history

            if(cmd[0] == '!' && cmd[1] != '!') {
                if(!loadSpecificCmd(hist_list, cmd, hist_num, args, right, &param_num_l, &param_num_r, &is_pipe)) {
                    continue;
                }
            }
            // Load a specific command by entry
            // Maximum entry is 1000

            if((strcmp(args[0], "history") == 0 && args[1] == NULL) ||
            (strcmp(args[0], "history") == 0 && strcmp(args[1], "&") == 0 && args[2] == NULL)) {
                should_show_history = 1;
            }
           
            int is_ampersand = 0;
            if(strcmp(args[param_num_l - 1], "&") == 0 && strcmp(args[0], "&") != 0) {
                should_wait = 0;
                args[param_num_l - 1] = NULL;
                --param_num_l;
                cmd[strlen(cmd) - 1] = '\0';
                is_ampersand = 1;
            }
            if(right[0] != NULL) {
                if(is_ampersand) {
                    printf("Misplace '&'.\n");
                    continue;
                }
                else if(strcmp(right[param_num_r - 1], "&") == 0 && strcmp(right[0], "&") != 0) {
                    should_wait = 0;
                    right[param_num_r - 1] = NULL;
                    --param_num_r;
                    cmd[strlen(cmd) - 1] = '\0';
                }
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

        int direction = isDirection(args, param_num_l);
        // Check for > or <

        switch(child) {
            case -1:
                printf("Fail to create a child process!\n");
                exit(EXIT_FAILURE);
                break;
            case 0:
                if(should_show_history) {
                    showHistory(hist_list, hist_num);
                }
                else if (is_pipe) {
                    execPipe(args, right);
                }
                else if (direction == 1) {
					int fd; 
					char* filename = (char*)malloc(sizeof(char) * strlen(args[param_num_l-1]));					
					mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;	
					
					strcpy(filename,args[param_num_l-1]);
					printf("Redirect > %s\n", filename);
					fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode); 
					if (fd < 0)
						{
							perror("open error");
							exit(0);
						}
					
					dup2(fd, STDOUT_FILENO);
					
					execCommand(args);
					close(fd);
				}
                else if (direction == 2) {
					int fd;
					char* filename = (char*)malloc(sizeof(char) * strlen(args[param_num_l-1]));						
					strcpy(filename,args[param_num_l-1]);
					printf("Redirect < %s\n", filename);
					fd = open(filename, O_RDONLY, 0); 
					if (fd < 0)
						{
							perror("open error");
							exit(0);
						}
					
					dup2(fd, STDIN_FILENO);

					execCommand(args);
					close(fd);
				}
                else execCommand(args);
                exit(EXIT_FAILURE);
                break;
            default:
                if(should_wait) {
                    wait(&child_status);
                }
                else {
                    printf("[%ld]\n", child);
                    waitpid(child, &child_status, WUNTRACED);
                    if(WIFEXITED(child_status)) {
                        printf("[%ld] has exited\n", child);
                    }
                }
        }
        
    }
    return 0;
}