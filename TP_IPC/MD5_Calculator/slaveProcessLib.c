#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "fileDescriptors.h"
#include "slaveProcessLib.h"

hashedFileADT calculateFileMD5Hash(char* fileName)
{
	int status = 0;
	hashedFileADT hashedFile = createHashedFile(fileName);

	int fileDescriptors[2] = {-1,-1};
	pipe(fileDescriptors);

	int pid = fork();
	if(pid == 0)
	{
		linkWriteEndOfPipeWithSTDOUT(fileDescriptors);
		executeMD5HashCommand(fileName);
	}

	linkReadEndOfPipeWithSTDIN(fileDescriptors);

	wait(&status);

	scanf("%s  %s", hashedFile->hash, hashedFile->filename);
	return hashedFile;
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

void sendHashedFileThroughPipe(int pipeFileDescriptor, hashedFileADT file)
{
	int hashedFileDataLength =  strlen(file->hash) + strlen(file->filename) + 2;
	char* dataToSend = malloc(hashedFileDataLength * sizeof(char));
	strncpy(dataToSend, file->filename, strlen(file->filename));
	dataToSend[strlen(file->filename)] = ':';
	strncpy(dataToSend, file->hash, strlen(file->hash));
	dataToSend[strlen(file->filename)] = '\n';
	write(pipeFileDescriptor, dataToSend, hashedFileDataLength);
	free(dataToSend);
}

void readFileFromPipe(int pipeFileDescriptor, char* fileNameBuffer)
{
	int i;
	char currentChar = 0;

	for(i = 0; currentChar != '\n'; i++)
	{
		read(pipeFileDescriptor, &currentChar, 1);
		fileNameBuffer[i] = currentChar;
	}
	fileNameBuffer[i] = '\0';
}
