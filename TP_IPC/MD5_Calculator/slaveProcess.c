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
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = terminate;
	sigaction(SIGTERM, &action, NULL);

	char* filenameBuffer = calloc(256, sizeof(char));
	hashedFileADT file;

	int readEndOfPipe = atoi(argv[1]);
	int writeEndOfPipe = atoi(argv[2]);
	while(!done)
	{
		readFileFromPipe(readEndOfPipe, filenameBuffer);
		if(strlen(filenameBuffer) > 0)
		{
			file = calculateFileMD5Hash(filenameBuffer);
			sendHashedFileThroughPipe(writeEndOfPipe, file);
		}
	}
	free(filenameBuffer);
}

void terminate(int signum)
{
  done = 1;
}
