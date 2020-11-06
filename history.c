#include "header.h"

void historySave(char *cmd, char *hist_list[], int *hist_num) {
    if(cmd[0] != '!') {
        hist_list[*hist_num] = (char*)malloc(sizeof(char) * (strlen(cmd) + 1));
        strcpy(hist_list[*hist_num], cmd);
        ++*hist_num;
    }
}

void showHistory(char *hist_list[], int hist_num) {
    for(int i = 0; i < hist_num; i++) {
        printf("%d\t%s\n", i + 1, hist_list[i]);
    }
}

int loadLastCmd(char *hist_list[], int hist_num, char *args[], char *right[], int *param_num_l, int *param_num_r, int *is_pipe) {
    if(hist_list[hist_num - 1][0] == '\0' || hist_num == 0) {
        printf("There is no previous command.\n");
        return 0;
    }
    else {
        *is_pipe = tokenizeCmd(hist_list[hist_num - 1], args, right, &*param_num_l, &*param_num_r);
        printf("%s\n", hist_list[hist_num - 1]);
        return 1;
    }
}

int loadSpecificCmd(char *hist_list[], char *cmd, int hist_num, char *args[], char *right[], int *param_num_l, int *param_num_r, int *is_pipe) {
    int entry_num = 0;
    for(int i = 1; i <= 4; i++) {
        if(cmd[i] != '\0' && cmd[i] >= '0' && cmd[i] <= '9') {
            entry_num = entry_num * 10 + (cmd[i] - '0');
        }
    }
    if(entry_num <= hist_num && entry_num >= 1) {
        *is_pipe = tokenizeCmd(hist_list[hist_num - 1], args, right, &*param_num_l, &*param_num_r);
        printf("%s\n", hist_list[entry_num - 1]);
        return 1;
    }
    else {
        printf("Event %s not found.\n", cmd);
        return 0;
    }
}