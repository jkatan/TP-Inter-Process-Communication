#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "slaveProcessLib.h"
#include "hashedfile.h"

volatile int done = 0;

int main(int argc, char const *argv[])
{
	initializeKillSignalHandler();

	char* filename = malloc(sizeof(char)*256);
	hashedFileADT file;

	int readEndOfPipe = (int)argv[1][0];
	int writeEndOfPipe = (int)argv[2][0];

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
