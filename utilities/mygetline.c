#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"getline.h"

size_t mygetline(char **contents, FILE* stream) {
    int size = 4;
    size_t i, count=2;
    char *final = (char*)malloc(size+1);
    fgets(final, size+1, stream);
    char *new = (char*) malloc(size+1);
    if((strlen(final) == size) && (final[size-1] != '\n')) {
        size_t index = size;
        while(final[index-1] != '\n') {
            final = (char*)realloc(final, (count++)*size);
            for(i=0; i<size; i++) {
                final[index++] = fgetc(stream);
                if(final[index-1] == '\n')
                    break;
            }
        }
            
    }
    //printf("%s", final);
    *contents = strdup(final);
    return strlen(final)-1;
}

/*int main(int argc, char*argv[]) {
    char *contents;
    size_t linelen;
    FILE* stream;
    if(argc > 1)
        stream = fopen(argv[1], "r");
    else
        stream = stdin;
    linelen = mygetline(&contents, stream);
    printf("length: %lu\n", linelen);
    printf("contents: %s\n", contents);
    return 0;
}*/
