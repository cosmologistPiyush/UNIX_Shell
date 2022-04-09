#include "helper.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

char** cmd;
bool redirection = false;

int main(int argc, char *argv[]) {
    char *line=NULL;
    size_t linecap=0, i, count;
    ssize_t linelen=0;
    int status;
	static size_t bckgrd_num;
    commands *all;
	static bool background;
	pid_t cpid;
    FILE* stream;
    char **path = (char**) malloc(sizeof(char*));
    if(!path) {
        printf("e: %i\n", errno);
        perror("malloc in the beginning of main failed");
        exit(1);
    }
    *path = NULL;

    if(argc == 2) {
        stream = fopen(argv[1], "r");
        if(stream == NULL) {
            perror("cant't open batch processing file");
            exit(1);
        }
    } else if(argc == 1)
        stream = stdin;
    else {
        perror("too many arguments");
        exit(1);
    }

    printf("wish> ");
    while((linelen = getline(&line, &linecap, stream)) != EOF) {

        assert(linelen > 0);

        if(*line == '\n') { //provides default shell like enter key functionality
            printf("wish> ");
            continue;
        }
        if(strcmp(line, "exit\n") == 0) //exit command
            break;

        all = effIpProcessing(&line, linelen-1);

		bckgrd_num = 0;

        for(i=0; i<all->num; i++) {
			count = 0;

            cmd = all->cmds[i];
            for(char**k = cmd; k && *k; k++)
                count++; //value of count is not exact. Doesn't include terminating NULL

            if(!((cmd[0][0] == '/') && (cmd[0][0] == '.'))) { // if a full path is not mentioned for executable
                if(strcmp(*cmd, "cd") == 0) {
                    if(cd(cmd, count) != 0) {
						freeEverything(all, path, line);
                        perror("cd failed"); // cmd is freed at the end
					}
                    continue;
                } else if(strcmp(*cmd, "path") == 0) {
                    if(count == 1) {
						if (!(*path))
							printf("\n");
						else {
							char** it = path;
							puts("the path is");
							for(; it && *it; it++)
								printf("%s\n", *it);
						}
						continue;
					}

					if (*path)
						freePath(path);
					int success = makePath(cmd, &path, count); //null terminated
					if(success != 0) {
						freeEverything(all, path, line);
						perror("setting path failed");
					}
					continue;
                } else
					defaultCommands(path); // i.e a shell command
            }

			if (strcmp(cmd[count-1], "&") == 0) {
				background = true;
				bckgrd_num += 1;

				if ((signal(SIGCHLD, child_handler)) == SIG_ERR) 
					perror("background command's signal handler");
			} else {
				if ((signal(SIGCHLD , SIG_DFL)) == SIG_ERR)
					perror("attempt to re-establish default signal handling");
			}

			cpid = fork();
			if(cpid < 0) {
                puts("wish: Fork error");
                exit(1);
			} if(cpid == 0) {

				if (redirection)
					redirect(count-1);

				if(background)
					printf("[%zi]\t%i\n", bckgrd_num, getpid());

                execv(*cmd, cmd);
                printf("e: %i\n", errno);
                perror("exec error");
                exit(127);
            } 
			if(background)
				continue;
			else {
				cpid = waitpid(cpid, &status, 0);
				assert(cpid > 0);
			}
        }

        printf("wish> ");
		freecmdstruct(all);

    }
	freeEverything(NULL, path, line);

    return 0;
}
