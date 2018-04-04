#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include "hashedFile.h"
#include "applicationProcessLib.h"
#include "queuelib.h"
#include "sharedMemory.h"



int calculateQuantityOfSlaveProcessesToCreate(int quantityOfFiles)
{
	return (int)ceil(quantityOfFiles/5.0);
}

void createSlaveProcesses(slaveADT * slaves, int quantityOfSlaves)
{
	int pipeToSlave[2] = {-1,-1};
	int pipeToApplication[2] = {-1,-1};
	int i, pid = getpid();

	for(i = 0; i < quantityOfSlaves; i++)
	{
		pipe(pipeToSlave);
		pipe(pipeToApplication);
		pid = fork();
		if(pid == 0)
		{
			close(pipeToSlave[1]);
			close(pipeToApplication[0]);

			char bufferArg1[3];
			char bufferArg2[3];
			sprintf(bufferArg1, "%d", pipeToSlave[0]);
			sprintf(bufferArg2, "%d", pipeToApplication[1]);

			char* arguments[] = {"./MD5_Calculator/slaveProcess" ,
								bufferArg1, //where slaveProcess must read
								bufferArg2, NULL}; //where slaveProcess must write
			execvp(arguments[0], arguments);
		}
		close(pipeToSlave[0]);
		close(pipeToApplication[1]);
		slaves[i] = createSlave(pipeToApplication[0],pipeToSlave[1], pid);
	}
	return;
}


slaveADT createSlave(int readFromSlave, int writeToSlave, int pid)
{
	slaveADT newSlave = (slaveADT) malloc(sizeof(slaveCDT));
	newSlave->writeTo = writeToSlave;
	newSlave->readFrom = readFromSlave;
	newSlave->filesGivenToProcess = 0;
	newSlave->slavePID = pid;
	return newSlave;
}

void terminateSlaves(slaveADT slaves[], int quantityOfSlaves)
{
	int i;
	for(i = 0; i < quantityOfSlaves; i++)
	{
		terminateSlave(slaves[i]);
	}
}

void terminateSlave(slaveADT slave)
{
	kill(slave->slavePID, SIGTERM);
	free(slave);
}

void sendFiles(slaveADT* slaves, int quantityOfSlaves, queueADT filesQueue)
{

	int i;
	for(i = 0; i < quantityOfSlaves; i++)
	{
		while(slaves[i]->filesGivenToProcess < MIN_QTY_FILES_TO_PROCESS && !isEmpty(filesQueue))
		{
			char* file = dequeueElement(filesQueue);
			send(slaves[i], file);
			slaves[i]->filesGivenToProcess++;
		}
	}
}


void send(slaveADT slave, char* file)
{
	int i;
	int fileLength = strlen(file);
	char* fileToSend = calloc((fileLength + 1) , sizeof(char));
	for(i = 0; i < fileLength; i++)
	{
		fileToSend[i] = file[i];
	}
	fileToSend[i] = '\n';
	write(slave->writeTo, fileToSend, fileLength + 1);
	free(fileToSend);

}

int receiveHashes(slaveADT* slaves,  int quantityOfSlaves, int* sharedMemoryAddress, int maxReadFileDescriptor,  int semaphoreId)
{
	int i, quantityOfHashesReceived = 0;
	fd_set fileDescriptorSetToReadFromSlaves;
	int selectRet;

	FD_ZERO(&fileDescriptorSetToReadFromSlaves);
	for(i = 0; i <  quantityOfSlaves; i++)
	{
		FD_SET(slaves[i]->readFrom, &fileDescriptorSetToReadFromSlaves);
	}
	selectRet = select(maxReadFileDescriptor, &fileDescriptorSetToReadFromSlaves, NULL, NULL, NULL);

	if(selectRet == -1)
	{
		terminateSlaves(slaves, quantityOfSlaves);
		perror("Failed to select() file descriptors from slave");
		exit(1);
	}

	for(i = 0; i < quantityOfSlaves && selectRet != 0; i++)
	{
		if(FD_ISSET(slaves[i]->readFrom, &fileDescriptorSetToReadFromSlaves))
		{
			quantityOfHashesReceived += receiveHash(slaves[i], sharedMemoryAddress, semaphoreId);
			slaves[i]->filesGivenToProcess--;
		}
	}
	return quantityOfHashesReceived;
}

int receiveHash(slaveADT slave, int* sharedMemoryAddress, int semaphoreId)
{
	int end = 0;
	accessSharedMemory(semaphoreId);
	int position = sharedMemoryAddress[0];
	while(!end && read(slave->readFrom,(sharedMemoryAddress+position), 1) >=0)
	{
		if(*(sharedMemoryAddress+position) == '\n')
		{
			end = 1;
		}
		position++;
	}
	sharedMemoryAddress[0] = position;
	leaveSharedMemory(semaphoreId);
	return 1;
}

int getMaxReadFileDescriptor(slaveADT* slaves, int quantityOfSlaves)
{
	int max = 0;
	int i = 0;
	for(i = 0; i < quantityOfSlaves; i++)
	{
	 if(max < slaves[i]->readFrom)
	 {
		 max = slaves[i]->readFrom;
	 }
	}
	return max;
}

void sendSharedMemoryDataToNewFile(char* newFileName, int* sharedMemoryAddress)
{
	FILE* newFile = fopen(newFileName, "w+");

	int position = 2;
	while (position < sharedMemoryAddress[0])
	{
  	fwrite(sharedMemoryAddress+position, 1, 1, newFile);
  	position++;
	}

	fclose(newFile);
}

void enqueueFiles(queueADT myQueue, char* files[], int numberOfFiles)
{
	for(int i = 0; i < numberOfFiles; i++)
	{
		if(isARegularFile(files[i]))
		{
			enqueueElement(myQueue, files[i]);
		}
	}
}

int isARegularFile(char* pathToFile)
{
	struct stat buffer;
  stat(pathToFile, &buffer);
  return S_ISREG(buffer.st_mode);
}
