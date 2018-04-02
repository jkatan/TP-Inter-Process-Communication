#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include "hashedFile.h"
#include "applicationProcessLib.h"


int calculateQuantityOfSlaveProcessesToCreate(int quantityOfFiles)
{
	return (int)ceil(quantityOfFiles/5.0);
}

void createSlaveProcesses(slaveADT * slaves, int quantityOfSlaves)
{
	printf("Program creating slaves... \n");
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
			printf("Slave create:%d \n", getpid());
			close(pipeToSlave[1]);
			close(pipeToApplication[0]);
			char* arguments[] = {"/MD5_Calculator/slaveProcess" ,
								(char*)&pipeToSlave[0], //where slaveProcess must read
								(char*)&pipeToApplication[1]}; //where slaveProcess must write
			execvp(arguments[0], arguments);
			printf("\n\n WHY ARE WE HERE ? JUST TO SUFFER?!\n\n");
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
}

int sendFiles(slaveADT slave, int nextFile, char** files, int quantityOfFiles, int quantityOfFilesToSend)
{
	int i;
	for(i = nextFile; i < quantityOfFilesToSend && i < quantityOfFiles ; i++)
	{
			send(slave,files[i]);
	}
	return i - nextFile;
}


void send(slaveADT slave, char* file)
{
	char* fileToSend = malloc((strlen(file)+1)*sizeof(char));
	char finishReadingFile[] = {'\n'};
	strcat(fileToSend, file);
	strcat(fileToSend, finishReadingFile);
	write(slave->writeTo, file, strlen(fileToSend));
}

int receiveHashes(slaveADT slave, hashedFileADT* hashes, int* sharedMemoryAddress, int* position)
{
	int i = 0;
	while(receiveHash(slave, hashes, i, sharedMemoryAddress, position) != -1)
	{
		i++;
	}

	return 0;
}

int receiveHash(slaveADT slave, hashedFileADT* hashes, int nextHashedFile, int* sharedMemoryAddress,  int* position)
{
		int i = 0, currentPosition = 0;
		char* buffer = malloc(1000 * sizeof(char));
		while(i < 1000 && read(slave->readFrom,(buffer+i), 1) != -1 && *(buffer+i) !='\n')
		{
			if(read(slave->readFrom,(buffer + i), 1) == -1){
				free(buffer);
				return -1;
			}

			i++;
		}
		//falta chequeo de que *position no se pase del tamaño de la shared memory
		if(*(buffer+i) =='\n')//last position of filename:hash
		{
			while(currentPosition < i){
				sharedMemoryAddress[*position + currentPosition] = *(buffer + currentPosition);
				currentPosition++;
			}
			*position += currentPosition;
			sharedMemoryAddress[*position] = '\n';
			*position += 1;
		}
		free(buffer);
		return 1;
}
