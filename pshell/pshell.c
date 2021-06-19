#include"helper.h"
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>

static char** cmd;
char error_msg[30] = "An error has occured\n";
bool redirection = false;

void defaultCommands(char** path) { //creates an absolute path for shell commands

    for(char **it = path; (it) && (*it); ++it) { //pointer variable iterates over path array
        size_t size = (size_t)(strlen(*cmd) + strlen(*it) + 2); // size of the full absolute path
        char check[size];
        snprintf(check, size, "%s/%s", *it, *cmd); // writes the new absolute path in check variable
        if(!access(check, X_OK)) { //checks if the file is executable
            free(*cmd);
            *cmd = strdup(check); // replaces file name with absolute path
            break;
        }
    }
}

void background(pid_t pid) {
    char *msg = NULL;
    sprintf(msg, "pid: %i\trunning in background", pid);
    write(STDERR_FILENO, "+[1]", 4);
    write(STDERR_FILENO, msg, sizeof(msg));
}

int main(int argc, char *argv[]) {
    char *line=NULL;
    size_t linecap=0, i, j, count;
    ssize_t linelen=0;
    int status;
    commands *all;
    cpid_t *pids;
    char **path = (char**) malloc(sizeof(char*));
    *path = NULL;
    if(!path) {
        printf("e: %i\n", errno);
        perror("malloc in the beginning of main failed");
        exit(1);
    }

    if(argc == 1) {
        printf("wish> ");
        while((linelen = getline(&line, &linecap, stdin)) != EOF) {
            if(*line == '\n') { //provides default shell like enter key functionality
                printf("wish> ");
                continue;
            }
            if(strcmp(line, "exit\n") == 0) //exit command
                exit(0);

            all = effIpProcessing(&line, linelen-1);
            pids = calloc(all->num, sizeof(*pids));

            for(i=0; i<all->num; i++) {

                cmd = all->cmds[i];
                for(char**k = cmd; k && *k; k++)
                    count++;
                //printf("count: %zi\n", count);

                if(!((cmd[0][0] == '/') && (cmd[0][0] == '.'))) { // if not a full mentioned path for executable
                    if(strcmp(*cmd, "cd") == 0) {
                        if(cd(cmd, count) != 0)
                            perror("cd failed"); // cmd is freed at the end
                        goto out;
                    }
                    else if(strcmp(*cmd, "path") == 0) {
                        int success = makePath(cmd, &path, count);
                        if(success != 0) {
                            perror("setting path failed");

                            if(success == ENOMEM) {
                                for(char**it = path; it && *it; it++)
                                    free(it);
                            }
                        }
                        goto out;
                    }
                    else
                        defaultCommands(path); // i.e a shell command
                }

                pids[i].pid = fork();
                if(pids[i].pid < 0) {
                    puts("wish: Fork error");
                    exit(1);
                } if(pids[i].pid == 0) {
                    if(strcmp(cmd[count-1], "&") == 0) {
                        pids[i].backgrd = true;
                        background(pids[i].pid);
                        free(cmd[count-1]);
                        cmd[count-1] = NULL;
                    }
                    execv(*cmd, cmd);
                    printf("e: %i\n", errno);
                    perror("exec error");
                    exit(127);
                } 
            }
            for(j=0; j<all->num; j++) {
                if(!pids[j].backgrd) {
                    pids[j].pid = wait(&status);
                    assert(pids[j].pid > 0);
                }
            }

out:
            count = 0;
            printf("wish> ");
            for(i=0; i<all->num; i++) {
                for(char** k = all->cmds[i]; k && *k; k++)
                    free(*k);
            }
            free(all->cmds);
            free(all);

        }
    } else if(argc == 2) {
        FILE* cmd = fopen(argv[1], "r");
        if(cmd == NULL) {
            write(STDERR_FILENO, error_msg, 30);
            exit(1);
        }

        return 0;
    } else {
        puts("errro");
        exit(1);
    }

    return 0;
}
