#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "hashedFile.h"
#include "applicationProcessLib.h"


int calculateQuantityOfSlaveProcessesToCreate(int quantityOfFiles)
{
	return (int)ceil(quantityOfFiles/5.0);
}

void createSlaveProcesses(slaveADT * slaves, int quantityOfSlaves)
{
	int pipeToSlave[2] = {-1,-1};
	int pipeToApplication[2] = {-1,-1};
	int i, pid;

	for(i = 0; i < quantityOfSlaves; i++)
	{
		pipe(pipeToSlave);
		pipe(pipeToApplication);
		pid = fork();
		if(pid == 0){
			close(pipeToSlave[1]);
			close(pipeToApplication[0]);
			char* arguments[] = {"slaveProcess" ,
														(char*)&pipeToSlave[0], //where slaveProcess must read
															(char*)&pipeToApplication[1]}; //where slaveProcess must write
			execvp(arguments[0], arguments);
		}
		close(pipeToSlave[0]);
		close(pipeToApplication[1]);
		slaves[i] = createSlave(pipeToApplication[0],pipeToSlave[1]);
	}
}


slaveADT createSlave(int readFromSlave, int writeToSlave)
{
	slaveADT newSlave = (slaveADT) malloc(sizeof(slaveCDT));
	newSlave->writeTo = writeToSlave;
	newSlave->readFrom = readFromSlave;
	newSlave->filesGivenToProcess = 0;
	return newSlave;
}

void terminateSlaves(slaveADT slaves[], int quantityOfSlaves)
{
	int i;
	for(i = 0; i < quantityOfSlaves; i++)
	{
		char terminationMessage[] = {EOF};
		send(slaves[i], terminationMessage);
	}
}

int sendFiles(slaveADT slave, int nextFile, char ** files, int quantityOfFiles, int quantityOfFilesToSend)
{
	int i;
	for(i = nextFile; i < quantityOfFilesToSend && i < quantityOfFiles ; i++)
	{
			send(slave,files[i]);
	}
	return i;
}


void send(slaveADT slave, char * file)
{
	write(slave->writeTo, file, strlen(file)+1);
}

int receiveHashes(slaveADT slave, hashedFileADT * hashes)
{
	int i = 0;
	while(receiveHash(slave, hashes, i) != -1)
	{
		i++;
	}
	return 0;
}

int receiveHash(slaveADT slave, hashedFileADT * hashes, int nextHashedFile)
{
		int i = 0, end = 0;
		char* filename = malloc(100 * sizeof(char));
		char* hash = malloc(33 * sizeof(char));
		char* buffer = malloc(1000 * sizeof(char));
		while(i < 1000 && read(slave->readFrom,(buffer+i), 1) != -1 && !end)
		{
			if(read(slave->readFrom,(buffer+i), 1) == -1){
				free(buffer);
				free(hash);
				free(filename);
				return -1;
			}
			if(*(buffer+i) ==' ')//hash and filename separator
			{
				*(buffer+i) = '\0';
				strncpy(hash, buffer, i+1);
				i = 0;
			}
			else if(*(buffer+i) =='\0')// hash, filname combo end
			{
				strncpy(hash, buffer, i+1);
				end = 1;
			}
			i++;
		}
		hashes[nextHashedFile] = createHashedFile(filename);
		hashes[nextHashedFile]->hash=hash;
		free(buffer);
		return 1;
}