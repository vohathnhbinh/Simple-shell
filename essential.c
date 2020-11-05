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

int tokenizeCmd(char *cmd, char* left[], char* right[], int *param_num_left, int *param_num_right) {
    char **next = left;
    char *dummy = (char*)malloc(sizeof(char) * (strlen(cmd) + 1));
    strcpy(dummy, cmd);

    char *temp = strtok(dummy, " ");
    while (temp != NULL)
    {
        
        *next++ = temp;
        
        //printf("%s\n", temp);
        temp = strtok(NULL, " ");
        ++*param_num_left;
        if (temp == NULL) break;
        if (strcmp(temp, "|") == 0) {
            break;
        }
    }

    *next = NULL;
    next = right;
    while (temp != NULL)
    {
        if (strcmp(temp, "|") == 0)
        {
            temp = strtok(NULL, " ");
            continue;
        }
        *next++ = temp;
        //printf("%s\n", temp);
        temp = strtok(NULL, " ");
        ++*param_num_right;
    }
    *next = NULL;
    if(right[0] == NULL) return 0;
    return 1;
}

int execPipe(char* left[], char* right[]) {
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }

    int pid1 = fork();
    if (pid1 < 0) {
        return 2;
    }
    if (pid1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        execCommand(left);
    }

    int pid2 = fork();
    if (pid2 < 0) {
        return 3;
    }
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execCommand(right);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void execCommand(char *args[]) {
    if(execvp(args[0], args) < 0) {
        printf("Executing command fails.\n");
    }
}

// debug
/*int main()
{
    char *cmd = (char *)malloc((MAX_LINE + 1) * sizeof(char));
    fgets(cmd, MAX_LINE + 1, stdin);
    cmd[strlen(cmd) - 1] = '\0';
    char *left[MAX_LINE];
    char *right[MAX_LINE];
    createPipe(cmd, left, right);
    execPipe(left, right);
}
*/