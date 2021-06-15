#include"helper.h"

int cd(char** input, size_t argc) {
    if(argc != 2) {
        perror("Number of arguments exceeded");
        return -1;
    }
    /*
     * check if the input path is valid
     */
    if(strcmp(input[1], "~") == 0) {
        free(input[1]);
        input[1] = strdup("/Users/piyushsachdeva");
        if(input[1] == NULL)
            return ENOMEM;
        return chdir(input[1]);
    }
    else if(input[1][0] == '.') { //make absolute path
        char *cwd = getcwd(NULL, 0);
#define size (strlen(cwd)+sizeof(input[1]))
        char path[size];
        snprintf(path, size, "%s/%s", cwd, input[1]);
        return chdir(path);
    }
    else //absolute path
        return chdir(input[1]);
    
}

