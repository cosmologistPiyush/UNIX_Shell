#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<stdbool.h>
#include<assert.h>

typedef struct inputsize {
    size_t strsincmd;
    size_t* stringsize;
}input;

typedef struct commands {
    char*** cmds;
    size_t num;
}commands;

typedef struct custom_pid_t {
    pid_t pid;
    bool backgrd;
} cpid_t;

extern bool redirection;
int cd(char**, size_t);
int makePath(char**,char***, size_t);

char*** creatingArray(commands* data, input* allcmds, char** line, size_t len);
commands* effIpProcessing(char** string, size_t len);
