#include"helper.h"

extern bool redirection;

commands* effIpProcessing(char** string, size_t len) {
    input_s *allcmds = malloc(sizeof(input_s));
    commands *data = malloc(sizeof(commands));
    if(!allcmds || !data)
        goto out;
    data->cmds = NULL;
    data->num = 1;
    size_t i;
    //allcmds[0].strsincmd = 0;

#define nc data->num-1
#define sic allcmds[nc].strsincmd
#define ss allcmds[nc].stringsize

    sic = 0;
    char* line = *string;

    /* calculating the amount of memory required */

    for(i=0; i<len; i++) {
        switch(line[i]) {
            case '&':
                sic += 2;
                ss = realloc(ss, sic*sizeof(size_t));
                if(ss == NULL)
                    goto out;
                ss[sic-2] = 1;
                ss[sic-1] = '\0';
                data->num += 1;
                allcmds = realloc(allcmds, data->num*sizeof(input_s));
                if(allcmds == NULL)
                    goto out;
                sic= 0;
                break;

            case '>':
                sic += 1;
                ss = realloc(ss, sic*sizeof(size_t));
                if(ss == NULL)
                    goto out;
                ss[sic-1] = 1;
                if(line[i+1] != ' ') {
                    sic+= 1;
                    ss = realloc(ss, sic*sizeof(size_t));
                    if(ss == NULL)
                        goto out;
                    ss[sic-1] = 0;
                }
                break;

            case ' ':
                if(line[i-1] == '&' || line[i+1] == '&' || line[i-1] == ' ' || line[i+1] == '>' || line[i+1] == '\0')
                    break;
                sic += 1;
                ss = realloc(ss, sic*sizeof(size_t));
                if(ss == NULL)
                    goto out;
                ss[sic-1] = 0;
                break;

            default:
                if(sic == 0) {
                    ss = malloc(sizeof(size_t));
                    if(ss == NULL)
                        goto out;
                    ss[0] = 0;
                    sic = 1;
                }
                ss[sic-1] += 1;
                break;
        }
    }

    if(ss[sic-1] != '\0') {
        sic += 1;
        ss = realloc(ss, sic*sizeof(size_t));
        if(ss == NULL)
            goto out;
        ss[sic-1] = '\0';
    }

    /* populating the array */
    if(sic == 0) {
        data->num -= 1;
        data->cmds = creatingArray(data, allcmds, string, len);
        data->num += 1;
    } else
        data->cmds = creatingArray(data, allcmds, string, len);

    /* freeing the memory */

    for(size_t i=0; i<nc; i++)
        free(allcmds[i].stringsize);
    free(allcmds);

    return data;

out:
    free(data);
    for(input_s* ip = allcmds; ip; ip++) {
        if(ip->stringsize)
            free(ip->stringsize);
    }
    free(allcmds);

    perror("realloc/malloc failed");
    exit(1);

#undef ss
#undef sic
}

char*** creatingArray(commands* data, input_s* allcmds, char** line, size_t len) {

    data->cmds = calloc(data->num, sizeof(char**));
    if(data->cmds == NULL)
        goto out;

    char*** cmd = data->cmds;

#define sic allcmds[i].strsincmd
#define ss allcmds[i].stringsize

    /*allocating the memory for the array*/
    size_t i, j;
    for(i=0; i<=nc; i++) {
        cmd[i] = calloc(sic, sizeof(char*));
        if(cmd[i] == NULL)
            goto out;
        for(j=0; ss[j] != '\0'; j++) {
            cmd[i][j] = calloc(ss[j], sizeof(char));
            if(cmd[i][j] == NULL)
                goto out;
        }
        cmd[i][j] = NULL;
    }

    /* filling the array memory */
    char* ptr = *line;
    i = j = 0;
    while(*ptr != '\n') {
        switch(*ptr) {
            case '&':
                j++;
                memcpy(cmd[i][j], ptr, ss[j]);
                i++;
                j=0;
                ptr++;
                break;

            case '>':
                j++;
                redirection = true;
                memcpy(cmd[i][j], ptr, ss[j]);
                if(*(ptr+1) != ' ')
                    j++;
                ptr++;
                break;

            case ' ':
                if(*(ptr-1) == '&' || *(ptr+1) == '&' || *(ptr-1) == ' ' || *(ptr+1) == '>' || *(ptr+1) == '\0') {
                }
                else
                    j++;
                ptr++;
                break;

            default:
                memcpy(cmd[i][j], ptr, ss[j]);
                ptr += ss[j];
                break;
        }
    }
    return cmd;

out:
    for(char*** ip = data->cmds; ip; ip++) {
        for(char** it = *ip; it && *it; it++)
            free(*it);
        free(*ip);
    }
    free(data->cmds);
    perror("calloc/malloc failed");
    exit(1);


#undef ss
#undef sic
#undef nc

}

/*int main() {
    char* line = NULL;
    size_t linecap=0, i;
    ssize_t linelen;
    commands *all;
    puts("input_s enter");
    linelen = getline(&line, &linecap, stdin);
    assert(linelen != 0);
    puts("processing begins");
    all = effIpProcessing(&line, linelen-1);

    char** j;

    for(i=0; i<all->num; i++) {
        for(j = all->cmds[i]; j && *j; j++) {
            printf("command %zi: --> ", i+1);
            printf("string: %s\n", *j);
        }
    }

    for(i=0; i<all->num; i++) {
        for(j = all->cmds[i]; j && *j; j++)
            free(*j);
    }
    printf("success\n");

    free(all->cmds);
    free(all);
    return 0;
}*/
