#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "slaveProcessLib.h"
#include "hashedFile.h"

volatile int done = 0;

int main(int argc, char const *argv[])
{
	printf("Hello from slave:%d \n", getpid());

	struct sigaction action;
  	memset(&action, 0, sizeof(struct sigaction));
  	action.sa_handler = terminate;
  	sigaction(SIGTERM, &action, NULL);

	char* filenameBuffer = malloc(sizeof(char)*256);
	hashedFileADT file;

	int readEndOfPipe = atoi(argv[1]);
	int writeEndOfPipe = atoi(argv[2]);

	printf("arg[1] received in slave: %d\n", readEndOfPipe);
	printf("arg[2] received in slave: %d\n", writeEndOfPipe);

	while(!done)
	{
		readFileFromPipe(readEndOfPipe, filenameBuffer);
		file = calculateFileMD5Hash(filenameBuffer);
		free(filenameBuffer);
		sendHashedFileThroughPipe(writeEndOfPipe, file);
		//falta liberar la estructura file;
	}
}

void terminate(int signum)
{
    done = 1;
}
