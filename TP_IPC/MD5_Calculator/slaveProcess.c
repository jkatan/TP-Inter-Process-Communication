#include "slaveProcess.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

char* calculateFileMD5Hash(char* fileName)
{
	int status = 0;

	char* hash = malloc(sizeof(char)*32);

	int fileDescriptors[2] = {-1,-1};
	pipe(fileDescriptors);

	int pid = fork();
	if(pid==0)
	{
		char *arguments[] = {"md5sum", fileName, NULL};
		close(fileDescriptors[0]);
		dup2(fileDescriptors[1], 1);
		close(fileDescriptors[1]);
		execvp(arguments[0], arguments);
	}
	close(fileDescriptors[1]);
	dup2(fileDescriptors[0], 0);
	close(fileDescriptors[0]);

	wait(&status);

	scanf("%s", hash);
	return hash;
} 