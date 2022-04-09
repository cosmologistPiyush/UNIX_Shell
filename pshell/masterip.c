#include "helper.h"

extern bool redirection;
static bool new_cmd = false;

commands* effIpProcessing(char** string, size_t len) {
	input_s *allcmds = malloc(sizeof(input_s));
    commands *data = malloc(sizeof(commands));
    if(!allcmds || !data)
        goto intermediateout;
    data->cmds = NULL;
    data->num = 1;
    size_t i;
    //allcmds[0].strsincmd = 0;

#define nc data->num-1
#define sic allcmds[nc].strsincmd
#define ss allcmds[nc].charsinstr

    sic = 0;
    char* line = *string;

    /* calculating the amount of memory required */
	new_cmd = true;

    for(i=0; i<len; i++) {
        switch(line[i]) {
            case '&':
				if (line[i-1] == ' ')
					sic += 1;
				else
					sic += 2;
				ss = realloc(ss, sic*sizeof(size_t));
				if(ss == NULL)
					goto intermediateout;
				ss[sic-2] = 1;
                ss[sic-1] = '\0';
				new_cmd = false;
                break;
			case ';':
				if (!(line[i-1] == ' '))
					sic += 1;
				ss = realloc(ss, sic*sizeof(size_t));
				if(ss == NULL)
					goto intermediateout;
                ss[sic-1] = '\0';
				new_cmd = false;
                break;

            case '>':
				if (!(line[i-1] == ' '))
					sic += 1;
				if(line[i+1] != ' ') {
					sic+= 1;
					ss = realloc(ss, sic*sizeof(size_t));
					if(ss == NULL)
						goto intermediateout;
					ss[sic-2] = 1;
					ss[sic-1] = 0;
					break;
				}
				ss = realloc(ss, sic*sizeof(size_t));
				if(ss == NULL)
					goto intermediateout;
                ss[sic-1] = 1;
                break;

            case ' ':
                if((!new_cmd) || line[i+1] == ' ')
                    break;
                sic += 1;
                ss = realloc(ss, sic*sizeof(size_t));
                if(ss == NULL)
                    goto intermediateout;

				ss[sic-1] = 0;

                break;

            default:
				if (!new_cmd) {
					data->num += 1;
					allcmds = realloc(allcmds, data->num*sizeof(input_s));
					if(allcmds == NULL)
						goto intermediateout;
					sic= 0;

					new_cmd = true;
				}

                if(sic == 0) {
                    ss = malloc(sizeof(size_t));
                    if(ss == NULL)
                        goto intermediateout;
                    ss[0] = 0;
                    sic = 1;
                }
                ss[sic-1] += 1;
                break;
			}
		}
		/* corner case of single word input */
	if (ss[sic-1] == 0)
		ss[sic-1] = '\0';
	if (ss[sic-1] != '\0') {
		sic += 1;
		ss = realloc(ss, sic*sizeof(size_t));
		if (ss == NULL)
			goto intermediateout;
		ss[sic-1] = '\0';
	}




    /* populating the array */
	data->cmds = creatingArray(data, allcmds, string, len);

    /* freeing the memory */

    for(size_t i=0; i<nc; i++)
        free(allcmds[i].charsinstr);
    free(allcmds);

    return data;

intermediateout:
    free(data);
    for(input_s* ip = allcmds; ip; ip++) {
        if(ip->charsinstr)
            free(ip->charsinstr);
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
        goto mainout;

    char*** cmds = data->cmds;

#define sic allcmds[i].strsincmd
#define ss allcmds[i].charsinstr

    /*allocating the memory for the array*/
    size_t i, j;
    for(i=0; i<=nc; i++) {
        cmds[i] = calloc(sic, sizeof(char*));
        if(cmds[i] == NULL)
            goto mainout;
        for(j=0; ss[j] != '\0'; j++) {
            cmds[i][j] = calloc(ss[j], sizeof(char));
            if(cmds[i][j] == NULL)
                goto mainout;
        }
        cmds[i][j] = NULL;
    }

    /* filling the array memory */
    char* ptr = *line;
    i = j = 0;
	new_cmd = false;
    while(*ptr != '\n') {
        switch(*ptr) {
            case '&':
                j++;
                memcpy(cmds[i][j], ptr, ss[j]);
			case ';':
				new_cmd = false;
				i++;
                ptr++;
                break;

            case '>':
				j++;
                redirection = true;
                memcpy(cmds[i][j], ptr, ss[j]);
                if(*(ptr+1) != ' ')
                    j++;
                ptr++;
                break;

            case ' ':
                if((!new_cmd) || *(ptr+1) == ' ' || *(ptr+1) == '&' || *(ptr+1) == '>') {
                }
                else
                    j++;
                ptr++;
                break;

            default:
				if (!new_cmd) {
					j=0;
					new_cmd = true;
				}
                memcpy(cmds[i][j], ptr, ss[j]);
                ptr += ss[j];
                break;
        }
    }
    return cmds;

mainout:
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
	puts("print done");

    for(i=0; i<all->num; i++) {
        for(j = all->cmds[i]; j && *j; j++)
            free(*j);
    }
	puts("freed indiv cmds");

    free(all->cmds);
    free(all);
    return 0;
}*/
