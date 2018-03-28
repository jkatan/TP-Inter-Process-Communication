#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "slaveProcess.h"
#include "fileDescriptors.h"
#include "queuelib.h"

char* calculateFileMD5Hash(char* fileName)
{
	int status = 0;

	char* hash = malloc(sizeof(char)*32);
	char* dump = malloc(sizeof(char)*1000);

	int fileDescriptors[2] = {-1,-1};
	pipe(fileDescriptors);

	int pid = fork();
	if(pid==0)
	{
		linkWriteEndOfPipeWithSTDOUT(fileDescriptors);
		executeMD5HashCommand(fileName);
	}

	linkReadEndOfPipeWithSTDIN(fileDescriptors);

	wait(&status);

	scanf("%s", hash);
	while(scanf("%s", dump) > 0);
	return hash;
}

void linkReadEndOfPipeWithSTDIN(int fileDescriptors[])
{
	close(fileDescriptors[1]);
	dup2(fileDescriptors[0], STDIN);
	close(fileDescriptors[0]);
}

void linkWriteEndOfPipeWithSTDOUT(int fileDescriptors[])
{
	close(fileDescriptors[0]);
	dup2(fileDescriptors[1], STDOUT);
	close(fileDescriptors[1]);
}

void executeMD5HashCommand(char* fileName)
{
	char *arguments[] = {"md5sum", fileName, NULL};
	execvp(arguments[0], arguments);
}
