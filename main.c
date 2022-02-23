#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <glob.h>
#include <pthread.h>
#include <utmpx.h>
#include <fcntl.h>
#include <sys/stat.h>

int sh(int argc, char** argv, char** envp) {
	char	commandline[MAXLINE];
	char	* args[MAXARGS];
	char	* ptr;
	char	* command, *wd, *owd, *lwd;
	pid_t	pid;
	int status, i, j, arg_no, background, redirection, append, pipe, rstdin, rstdout, rstderr;
	struct pathelement* dir_list, * tmp;
	char* cmd;
	int noclobber = 0;
	if ((wd = getcwd(NULL, PATH_MAX + 1)) == NULL){
		perror("getcwd");
		exit(2);
	}
	owd = calloc(strlen(wd) + 1, sizeof(char));
	memcpy(owd, wd, strlen(wd));
	lwd = owd; //Init last work directory

	fprintf(stdout, "[%s]> ", wd);
	fflush(stdout); //print prompt

	while ((fgets(commandline, MAX_CANON, stdin)) != NULL){ //parse command line
		if (commandline[strlen(commandline) - 1] == '\n') {
			commandline[strlen(commandline) - 1] = '\0';
		}
		args[0] = command = strtok(commandline, " ");
		if (!args[0]) { //if no args, break
			break;
		}
		i = 1;
		while (command != NULL && i < MAXARGS) {
			pch = strtok(NULL, " ");
			args[i] = command;
			i++;
		}
		args[i + 1] = (char*)NULL;

		if (strcmp(args[0], "exit") == 0) {
			printf("Exiting\n");
			free(owd);
			free(lwd);
			free(wd);
			exit(0)
		}
		else if (strcmp(args[0], "echo") == 0) {
			for (j = 1; j < i; j++) {
				printf("%d", args[j]);
				printf(" ");
			}
			printf("\n");
		}
		else if (strcmp(args[0], "which") == 0) {

		}
		else if (strcmp(args[0], "where") == 0) {

		}
		else if (strcmp(args[0], "cd") == 0) {
			char* homedir = getenv("HOME");
			if (args[2]) {
				printf("Too many arguments!\n");
				break;
			}
			if (!args[1]) {
				chdir(homedir);
				lwd = owd;
				owd = homedir;
			}
			else if (strcmp(args[1], "-") == 0){
				chdir(lwd);
				lwd = owd;
				owd = getcwd(NULL, 0);
			}
			else{
				if (chdir(args[1]) == 0) { //If successful CD
					lwd = owd;
					owd = getcwd(NULL, 0);
				}
				else { //Error
					printf("Invalid Argument\n");
				}
			}
		}
		else if (strcmp(args[0], "pwd") == 0) {

		}
		else if (strcmp(args[0], "ls") == 0) {
			printf("[Executing built-in list]\n");
			if (!args[1]) { //no args exist
				ptr = getcwd(NULL, 0);
				list(ptr);
			}
			else {
				int i = 1;
				while (args[i]) {
					printf("%s:\n", args[i]);
					list(args[i]);
					i++;
				}
			}
			break;
		}
		else if (strcmp(args[0], "pid") == 0) {
			printf("[Executing built-in pid]\n");
			int iptr = getpid();
			printf("Shell PID: %d\n", iptr);
			break;
		}
		else if (strcmp(args[0], "kill") == 0) {
			if (args[1]) {
				int k = -1;
				if (!args[2]) {
					k = kill(atoi(args[1]), SIGTERM);
				}
				else if (args[2]) {
					k = kill(atoi(args[2]), abs(atoi(args[1])));
				}
				if (k < 0) {
					printf("Kill: Failed: %d\n", k);
				}
			}
			else {
				printf("Invalid Arguments\n");
				break;
			}
		}
		else if (strcmp(args[0], "printenv") == 0) {
			if (args[3]) {
				printf("Invalid Input\n");
				goto Endprintenv
			}
			if (!args[1]) {
				int i = 1;
				while (envp[i] != NULL) {
					printf("%s\n", envp[i++]);
				}
			}
			else if (args[1]) {
				char* temp;
				if ((temp = getenv(args[1])) != NULL) {
					printf("%s\n", temp);
				}
				else {
					fprintf(stderr, "Error: Invalid Input\n");
				}
			}
		Endprintenv:
		}
		else if (strcmp(args[0], "watchuser") == 0) {

		}
		else if (strcmp(args[0], "noclobber") == 0) {
			printf("[Executing Built-in noclobber]\n");
			if (args[1]) {
				printf("Error: Invalid Input\n");
				break;
			}
			else if (noclobber == 1) {//set Noclobber to 0
				noclobber = 0;
				printf("Value of noclobber is %d\n", noclobber);
			}
			else {//set Noclobber to 1
				noclobber = 1;
				printf("Value of noclobber is %d\n", noclobber);
			}
		}
		}
		else { //program to execute

		}
	}//While
	exit(0);
} //end
