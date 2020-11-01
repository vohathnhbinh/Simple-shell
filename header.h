#ifndef HEADER_FILE
#define HEADER_FILE

// Essential
void splitCommand(char *cmd, char *args[], int *param_num);

// History
void historySave(char *cmd, char *hist_list[], int *hist_index);
void showHistory(char *hist_list[], int hist_index);
void loadLastCmd(char *hist_list[], int hist_index, char *args[], int param_num);

#endif