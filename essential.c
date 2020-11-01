#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MAX_LINE 80

void splitCommand(char *cmd, char *args[], int *param_num) {
    int i = 0;
    char *pch = strtok(cmd, " ");
    while(pch != NULL) {
        args[i] = pch;
        pch = strtok(NULL, " ");
        *param_num = ++i;
    }
    args[i] = NULL;
}