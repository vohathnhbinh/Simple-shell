#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* readCommand(char *args[]) {
    char *str, *p;
    str = (char *)malloc((2097152+1) * sizeof(char));
    fgets(str, 2097152, stdin);
    char *temp;
    temp = str;

    int i = 1;
    args[0] = p = strtok(str, " \n\t");
    while(p != NULL) {
        args[i] = p = strtok(NULL, " \n\t");
        i++;
    }
    free(str);
    return temp;
    // Return the whole command line.
}

int shouldWait(char *args[]) {
    int i = 0;
    while(args[i]) {
        if(args[i] == "&") {
            args[i] = NULL;
            return 1;
        }
        i++;
    }
    return 0;
}
/* Check whether or not the parent process is to wait for the child to exit.
If yes => replace "&" with NULL and return true.
else => return false. */
