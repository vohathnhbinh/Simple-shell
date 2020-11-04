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

int isDirection(char *arg[], int n) {
    int count = 0, returns = 0;
    for (int i = 0; i < n-1; i++) {
        if ((strcmp(arg[i],">") == 0)) {
			argv[i]=NULL;
            returns = 1; 
            count += 1;
        }
        else if ((strcmp(arg[i],"<") == 0)) {
			argv[i]=NULL;
            returns = 2;
            count += 1;
        }
    }
    if (count != 1) return -1;
    return returns;
}

