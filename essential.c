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

void createPipe(char *cmd, char* left[], char* right[]) {
    char **next = left;

    char *temp = strtok(cmd, " ");
    while (temp != NULL)
    {
        
        *next++ = temp;
        
        //printf("%s\n", temp);
        temp = strtok(NULL, " ");
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
    }
    *next = NULL;
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
        execvp(left[0], left);
    }

    int pid2 = fork();
    if (pid2 < 0) {
        return 3;
    }
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(right[0], right);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

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