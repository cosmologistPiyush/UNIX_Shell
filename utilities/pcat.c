#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include"getline.h"

int main(int argc, char **argv) {
    size_t i, linelen;
    char* line;
    if(argc == 1) { // case for no arguments. prints the input string on the next line.
        while((linelen = mygetline(&line, stdin)) != EOF) {
            printf("%s", line);
        }
    }
    else { // works with more than one file and prints their contents in order.
        for(i=1;i<argc;i++) {
            FILE *f = fopen(argv[i], "r");
            if(f == NULL) {
                printf("Error: %d\n", errno);
                printf("The file, '%s' can not be opened: %s\n", argv[i], strerror(errno));
                puts("wcat: cannot open file");
                exit(1);
                }
            while((linelen = mygetline(&line, f)) != EOF) {
                printf("%s", line);
            }
            fclose(f);
        }
    }
    return 0;
}

