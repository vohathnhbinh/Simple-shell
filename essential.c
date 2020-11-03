#include "header.h"

#define MAX_LINE 80

void splitCommand(char *cmd, char *args[], int *param_num) {
    int i = 0;
    
    char *temp = (char*)malloc(sizeof(char) * (strlen(cmd) + 1));
    strcpy(temp, cmd);
    // Allocate memory for temp to not affect cmd
    
    char *pch = strtok(temp, " ");
    while(pch != NULL) {
        args[i] = pch;
        pch = strtok(NULL, " ");
        *param_num = ++i;
    }
    for(; i < MAX_LINE/2 + 1; i++) {
        args[i] = NULL;
    }
}

void childHandler(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
}