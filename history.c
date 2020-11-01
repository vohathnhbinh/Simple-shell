#include "header.h"
#include <string.h>
#include <stdio.h>

void historySave(char *cmd, char *hist_list[], int *hist_index) {
    if(cmd[0] != '!') {
        hist_list[*hist_index] = cmd;
        ++*hist_index;
    }
}

void showHistory(char *hist_list[], int hist_index) {
    for(int i = 0; i < hist_index; i++) {
        printf("%d\t%s\n", i + 1, hist_list[i]);
    }
}

void loadLastCmd(char *hist_list[], int hist_index, char *args[], int param_num) {
    if(hist_list[hist_index - 1][0] == '\0') {
        printf("There is no previous command.\n");
    }
    else if(strlen(hist_list[hist_index - 1]) == 1 &&
            hist_list[hist_index - 1][0] == '&') {
        return;
    }
    else {
        splitCommand(hist_list[hist_index - 1], args, &param_num);
    }
}