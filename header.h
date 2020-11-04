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

#define MAX_LINE 80

// Essential
void splitCommand(char *cmd, char *args[], int *param_num);
void childHandler(int sig);

// History
void historySave(char *cmd, char *hist_list[], int *hist_num);
void showHistory(char *hist_list[], int hist_num);
int loadLastCmd(char *hist_list[], int hist_num, char *args[], int *param_num);
int loadSpecificCmd(char *hist_list[], char *cmd, int hist_num, char *args[], int *param_num);

#endif