#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "applicationProcessLib.h"
#include <stdlib.h>



int main(int argc, char const* argv[])
{
	int i, quantityOfFilesSent, quantityOfHashesReceived, nextFile = 0, quantityOfFiles = argc;
	char ** files = (char **)argv;

	hashedFileADT * hashes = NULL;

	int quantityOfSlaves = calculateQuantityOfSlaveProcessesToCreate(quantityOfFiles);

	slaveADT slaves[quantityOfSlaves];
	createSlaveProcesses(slaves, quantityOfSlaves);

	while(nextFile < quantityOfFiles)
	{
		for(i = 0; i < quantityOfSlaves; i++)
		{
			quantityOfHashesReceived = receiveHashes(slaves[i], hashes);
			slaves[i]->filesGivenToProcess -= quantityOfHashesReceived;

			if(slaves[i]->filesGivenToProcess < MIN_QTY_FILES_TO_PROCESS)
			{
				quantityOfFilesSent = sendFiles(slaves[i], nextFile, files, quantityOfFiles, QTY_TO_SEND);
				slaves[i]->filesGivenToProcess += quantityOfFilesSent;
				nextFile += quantityOfFilesSent;
			}


		}
	}
	terminateSlaves(slaves, quantityOfSlaves);

}
