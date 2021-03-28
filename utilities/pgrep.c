#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include"getline.h"

int main(int argc, char **argv) {
    size_t i;
    char* search;
    char* contents = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    if(argc > 2) { //standard operation of searching given string through all the mentioned files
        search = argv[1];
        if(!(strncmp(search, " ", strlen(search)))) {
            puts("NO Match");
            return 0;
        }
        for(i=2;i<argc;i++) {
            FILE *f = fopen(argv[i], "r");
            if(f == NULL) {
                puts("wgrep: cannot open file");
                exit(1);
            }
            while((linelen = getline(&contents, &linecap, f)) != EOF) {
                if(strstr(contents, search))
                    printf("%s", contents);
            }
        }
    }
    else if(argc == 2) { // with only the search term mentioned, pgrep reads through the std input
        search = argv[1];
        while((linelen = mygetline(&contents, stdin)) != 0) {
            if(strstr(contents, search))
                printf("%s", contents);
        }
    }
    else {
        puts("wgrep: searchterm [file ...]");
        exit(1);
    }
    return 0;
}

