#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<stdbool.h>
#include<assert.h>

typedef struct inputsize {
    size_t strsincmd;
    size_t* charsinstr; //treated as an array. stringsize[n] = characters in (n+1)th str.
}input_s;

typedef struct commands {
    char*** cmds;
    size_t num;
}commands;

typedef enum command_type { NORMAL, MULTIPLE, BACKGROUND } cmd_type;

typedef struct custom_pid_t {
    pid_t pid;
    cmd_type type;
} cpid_t;

extern int redirection;
int cd(char**, size_t);
int makePath(char**,char***, size_t);

char*** creatingArray(commands*, input_s*, char**, size_t);
commands* effIpProcessing(char**, size_t);
