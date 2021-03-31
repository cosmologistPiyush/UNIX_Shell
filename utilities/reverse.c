#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int compare(char** input, char** output, size_t c) {
    size_t i;
    size_t flag = 1;
    for(i=0; i<c; i++) {
        if(strcmp(input[i], output[i]) != 0) {
            flag = 0;
            break;
        }
    }
    return flag;
}

int main(int argc, char **argv) {
    size_t flag;
    char* line;
    size_t linecap=0, i=0, j, count=0;
    ssize_t linelen;
    char** ipContents = (char**)malloc(1);
    char** opContents = (char**)malloc(1);

    if(argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
        }

    else if(argc == 3) {
        FILE *input = fopen(argv[1], "r");
        if(input == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        FILE *output = fopen(argv[2], "w+");
        if(output == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
        while((linelen = getline(&line, &linecap, input)) != EOF) {
            count++;
            ipContents = (char**)realloc(ipContents, count*sizeof(char*));
            if(ipContents == NULL) {
                fprintf(stderr, "reverse: realloc failed\n");
                exit(1);
            }
            ipContents[i++] = strdup(line);
            if(ipContents == NULL) {
                fprintf(stderr, "reverse: malloc failed\n");
                exit(1);
            }
        }
        opContents = (char**) calloc(count, sizeof(char*));
            if(opContents == NULL) {
                fprintf(stderr, "reverse: calloc failed\n");
                exit(1);
            }
        for(j=0; j<count; j++) {
            --i;
            opContents[j] = strdup(ipContents[i]);
            if(opContents == NULL) {
                fprintf(stderr, "reverse: malloc failed\n");
                exit(1);
            }
        }

        flag = compare(ipContents, opContents, count);
        if(flag == 0) {
            for(i=0; i<count; i++) 
                fputs(opContents[i], output);
        } else {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
        fclose(output);
        fclose(input);

    }

    else if(argc == 2) {
        FILE *input = fopen(argv[1], "r");
        if(input == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        while((linelen = getline(&line, &linecap, input)) != EOF) {
            count++;
            ipContents = (char**)realloc(ipContents, count*sizeof(char*));
            if(ipContents == NULL) {
                fprintf(stderr, "reverse: realloc failed\n");
                exit(1);
            }
            ipContents[i++] = strdup(line);
            if(ipContents == NULL) {
                fprintf(stderr, "reverse: malloc failed\n");
                exit(1);
            }
        }
        opContents = (char**) calloc(count, sizeof(char*));
        if(opContents == NULL) {
            fprintf(stderr, "reverse: calloc failed\n");
            exit(1);
        }
        for(j=0; j<count; j++) {
            --i;
            opContents[j] = strdup(ipContents[i]);
            if(opContents == NULL) {
                fprintf(stderr, "reverse: malloc failed\n");
                exit(1);
            }
        }

        flag = compare(ipContents, opContents, count);
        if(flag == 0) {
            for(i=0; i<count; i++) 
                fputs(opContents[i], stdout);
        } else {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
        fclose(input);
    }

    else {
        while((linelen = getline(&line, &linecap, stdin)) != EOF) {
            count++;
            ipContents = (char**)realloc(ipContents, count*sizeof(char*));
            if(ipContents == NULL) {
                fprintf(stderr, "reverse: realloc failed\n");
                exit(1);
            }
            ipContents[i] = (char*) malloc(linelen);
            if(ipContents == NULL) {
                fprintf(stderr, "reverse: malloc failed\n");
                exit(1);
            }
            strcpy(ipContents[i++], line);
        }
        opContents = (char**) calloc(count, sizeof(char*));
        if(opContents == NULL) {
            fprintf(stderr, "reverse: calloc failed\n");
            exit(1);
        }
        for(j=0; j<count; j++) {
            --i;
            opContents[j] = (char*) malloc(sizeof(ipContents[i]));
            if(opContents == NULL) {
                fprintf(stderr, "reverse: malloc failed\n");
                exit(1);
            }
            strcpy(opContents[j], ipContents[i]);
        }

        flag = compare(ipContents, opContents, count);
        if(flag == 0) {
            for(i=0; i<count; i++) 
                fputs(opContents[i], stdout);
        } else {
            fprintf(stderr, "Input and Output file must differ\n");
            exit(1);
        }
    }
    
    for(i=0; i<count; i++) {
        free(ipContents[i]);
        free(opContents[i]);
    }
    return 0;
}
