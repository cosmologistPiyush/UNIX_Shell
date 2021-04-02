#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"path.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

int main(int argc, char *argv[]) {
    char *line, *commands;
    size_t linecap;
    ssize_t linelen;
    pid_t pid;
    /*char path[][] = {"/bin",
                      "/usr/bin/"};*/
    char **input = (char**) malloc(0); 
    
    if(argc == 1) {
        printf("wish> ");
        while((linelen = getline(&line, &linecap, stdin)) != EOF) {
            ssize_t i=0, count=0;
            if(line[linelen-1] == '\n')
                line[linelen-1] = '\0';
            if((linelen == 5) && (strcmp(line, "exit") == 0))
                exit(0);

            do {
                count++;
                input = (char**)realloc(input, count*sizeof(char*));
                input[i++] = strdup(strsep(&line, " "));
                
            } while(line !=  NULL);
            input[i] = NULL;
            pid = fork();
            int status;
            if(pid < 0) {
                puts("wish: Fork error");
                exit(1);
            } else if(pid == 0) {
                execv(input[0], input);
                printf("e: %i\n", errno);
                perror("Error");
                //printf("wish> ");
                break;
            } else {
                //pid = waitpid(pid, NULL);
                waitpid(pid, &status, 0);
                fflush(stdin);
                printf("wish> ");
            }
        }
    } else if(argc == 2) {
        return 0;
    } else {
        puts("errro");
        exit(1);
    }

    return 0;
}

