#include"helper.h"

int makePath(char **input, char*** path, size_t count) {
    char **path_p = *path;
    if((path_p = realloc(path_p, count*sizeof(char*))) == NULL) {
        perror("realloc for path failed");
        return -1;
    }
    size_t j = 0;
    for(size_t i = 1; i<count; i++)
        path_p[j++] = strdup(input[i]);
    path_p[j] = NULL;
    
    if(*path_p == NULL)
        return -1;
    
    return 0;
}
