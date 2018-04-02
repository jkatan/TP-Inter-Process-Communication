#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "slaveProcessLib.h"
#include "hashedFile.h"

volatile int done = 0;
void initializeKillSignalHandler()
{

}

int main(int argc, char const *argv[])
{
	initializeKillSignalHandler();

	char* filename = malloc(sizeof(char)*256);
	hashedFileADT file;

	int readEndOfPipe = atoi(argv[1]);
	int writeEndOfPipe = atoi(argv[2]);

	while(!done)
	{
		readFileFromPipe(readEndOfPipe, filename);
		file = calculateFileMD5Hash(filename);
		sendHashedFileThroughPipe(writeEndOfPipe, file);
	}
}

void terminate(int signum)
{
    done = 1;
}
