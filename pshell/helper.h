#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <assert.h>

typedef struct inputsize {
    size_t strsincmd;
    size_t* charsinstr; //treated as an array. stringsize[n] = characters in (n+1)th str.
}input_s;

typedef struct commands {
    char*** cmds;
    size_t num;
}commands;

typedef struct custom_pid_t {
    pid_t pid;
    bool background;
} cpid_t;

extern bool redirection;
extern char **cmd;

int cd(char**, size_t);
int makePath(char**,char***, size_t);

char*** creatingArray(commands*, input_s*, char**, size_t);
commands* effIpProcessing(char**, size_t);
void freePath(char**);
void defaultCommands(char**);

void child_handler(int);
void redirect(size_t);
void freecmdstruct(commands*);
void freeEverything(commands*, char **, char*);
