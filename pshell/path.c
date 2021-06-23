#include"helper.h"

int makePath(char **input, char*** path, size_t count) {
    char **path_p = NULL;
    path_p = *path;
    path_p = realloc(path_p, count*sizeof(char*));
    if(!path_p) {
        free(path_p);
        perror("realloc for path failed");
        return -1;
    }
    size_t i = 1;
    for(; i<count; i++) {
        path_p[i-1] = NULL;
        path_p[i-1] = strdup(input[i]);
        if(path_p[i-1] == NULL)
            return ENOMEM;
    }
    path_p[i-1] = NULL;
    
    return 0;
}
