#include "header.h"

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

int isDirection(char *args[], int n) {
    int count = 0, returns = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(args[i],">") == 0) {
			args[i]=NULL;
            returns = 1; 
            ++count;
        }
        else if (strcmp(args[i],"<") == 0) {
			args[i]=NULL;
            returns = 2;
            ++count;
        }
    }
    if (count != 1) return -1;
    return returns;
}

