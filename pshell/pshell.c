#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"path.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

void defaultCommands(char*** input, char** path) { //creates an absolute path for shell commands
    char **ip = *input; // gets the name of file/command

    for(char **it = path; (it) && (*it); ++it) { //pointer variable iterates over path array
        size_t size = (size_t)(strlen(*ip) + strlen(*it) + 2); // size of the full absolute path
        char check[size];
        snprintf(check, size, "%s/%s", *it, *ip); // writes the new absolute path in check variable
        if(!access(check, X_OK)) { //checks if the file is executable
            free(*ip);
            *ip = strdup(check); // replaces file name with absolute path
            printf("cmd: %s\n", *ip);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    char *line=NULL;
    size_t linecap=0, count, i;
    ssize_t linelen=0;
    int status;
    pid_t pid;
    char **input = (char**) malloc(sizeof(char*)); 
    char **path = (char**) malloc(4*sizeof(char const*));
    char **temp = path;
    *temp++ = NULL;
    *temp++ = strdup("/usr/bin");
    *temp++ = strdup("/bin");
    *temp = NULL;
    if(argc == 1) {
        printf("wish> ");
        while((linelen = getline(&line, &linecap, stdin)) != EOF) {
            i=count=0;
            if(*line == '\n') { //provides default shell like enter key functionality
                printf("wish> ");
                continue;
            }
            if(line[linelen-1] == '\n')
                line[linelen-1] = '\0';
            if((linelen == 5) && (strcmp(line, "exit") == 0)) //exit command
                exit(0);
            *path = strdup(getcwd(NULL, 0)); // adds current working directory to path

            do {
                count++; // counter for number of char pointers
                input = (char**)realloc(input, count*sizeof(char*));
                input[i++] = strdup(strsep(&line, " ")); // seperate contents of line by space pointed to by an array of char pointers
            } while(line !=  NULL);
            input[i] = NULL; // last pointer to null

            if(!(input[0][0] == '/')) // if not a full mentioned path for executable
                defaultCommands(&input, path); // i.e a shell command

            pid = fork();
            if(pid < 0) {
                puts("wish: Fork error");
                exit(1);
            } else if(pid == 0) {
                printf("cmd: %s\nargs: %s\n", *input, input[1]);
                execv(*input, input);
                printf("e: %i\n", errno);
                perror("Error");
                exit(127);
            } else {
                pid = waitpid(pid, &status, 0);
                if(pid > 0) {
                    printf("wish> ");
                    for(size_t i=0; i<count; i++)
                        free(input[i]);
                } else
                    perror("Error");
            }
            fflush(stdin);
        }
    } else if(argc == 2) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            perror("e");
            exit(1);
        }

        return 0;
    } else {
        puts("errro");
        exit(1);
    }

    return 0;
}

