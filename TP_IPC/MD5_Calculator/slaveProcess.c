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
	while(!done)
	{
		readFileFromPipe(readEndOfPipe, filenameBuffer);


		file = calculateFileMD5Hash(filenameBuffer);
		printf("%s:%s  (CALCULTED FROM SLAVE:%d)\n",file->hash, file->filename, getpid());

		sendHashedFileThroughPipe(writeEndOfPipe, file);
		//falta liberar la estructura file;
	}
	free(filenameBuffer);
}

void terminate(int signum)
{
    done = 1;
}
