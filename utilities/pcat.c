#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc, char **argv) {
    char contents[100];
    int i;
    if(argc == 1) { // case for no arguments. prints the input string on the next line.
        while(fgets(contents, 100, stdin)) {
            printf("%s", contents);
        }
        exit(0);
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
            while(fgets(contents, 100, f)) {
                printf("%s", contents);
            }
            fclose(f);
        }
    }
    return 0;
}

