#include "helper.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

extern char **cmd;

void defaultCommands(char** path) { //creates an absolute path for shell commands

    for(char **it = path; (it) && (*it); ++it) { //pointer variable iterates over path array
        size_t size = (size_t)(strlen(*cmd) + strlen(*it) + 2); // size of the full absolute path
        char check[size];
        snprintf(check, size, "%s/%s", *it, *cmd); // writes the new absolute path in check variable
        if(!access(check, X_OK)) { //checks if the file is executable
            free(*cmd);
            *cmd = strdup(check); // replaces file name with absolute path
            break;
        }
    }
}

void child_handler(int signo) { // signal handler for background processes 
    char msg[20];
	int status;
	pid_t pid = wait(&status);
    sprintf(msg, "pid: %i\tdone\n", pid);
    write(STDERR_FILENO, msg, sizeof(msg));
	printf("wish> ");
}


void redirect(size_t cmd_len) {
	/*
	 * cmd_len is 1 less than count (ignores the last NULL)
	 * therefore output file name is at index val: cmd_len
	 * and operator is at index value: cmd_len-1
	*/

	if (strcmp(cmd[cmd_len - 1], ">") != 0)
		return;

	int outputfile;

	redirection = false;

	outputfile = open(cmd[cmd_len], O_CREAT|O_WRONLY|O_TRUNC, S_IWUSR|S_IRUSR|S_IRGRP|S_IWGRP);
	assert(outputfile != -1);
	dup2(outputfile, STDOUT_FILENO);
	dup2(outputfile, STDERR_FILENO);
	free(cmd[cmd_len]);
	free(cmd[cmd_len - 1]);
	cmd[cmd_len - 1] = NULL;
	cmd = realloc(cmd, cmd_len*sizeof(char*));

}
        
void freecmdstruct(commands *data) {
	int i = 0;
	for (; i< data->num; i++) {
		for(char **j = data->cmds[i]; j && *j; j++)
			free(*j);
	}
	free(data->cmds);
	free(data);
}

void freeEverything(commands *all, char **path, char *line){
	if (*line)
		free(line);
	if (all)
		freecmdstruct(all);
	if (*path)
		freePath(path);
}
