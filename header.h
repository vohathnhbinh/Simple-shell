#ifndef HEADER_FILE
#define HEADER_FILE

#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 80

// Essential + Direction
void splitCommand(char *cmd, char *args[], int *param_num);
void childHandler(int sig);
int isDirection(char *args[], int n);
int tokenizeCmd(char *cmd, char* left[], char* right[], int *param_num_left, int *param_num_right);
int execPipe(char* left[], char* right[]);
void execCommand(char *args[]);
// History
void historySave(char *cmd, char *hist_list[], int *hist_num);
void showHistory(char *hist_list[], int hist_num);
int loadLastCmd(char *hist_list[], int hist_num, char *args[], char *right[], int *param_num_l, int *param_num_r, int *isPipe);
int loadSpecificCmd(char *hist_list[], char *cmd, int hist_num, char *args[], char *right[], int *param_num_l, int *param_num_r, int *isPipe);

#endif