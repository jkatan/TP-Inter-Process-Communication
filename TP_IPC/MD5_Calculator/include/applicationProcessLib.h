#ifndef APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#define MAX_QTY_FILES 200
#define MIN_QTY_FILES_TO_PROCESS 4
#define QTY_TO_SEND 3

#include "hashedFile.h"
#include "sharedMemory.h"
#include "semaphore.h"

typedef struct slaveCDT* slaveADT;

typedef struct slaveCDT
{
	int readFrom;
	int writeTo;
	int filesGivenToProcess;
	int slavePID;
}slaveCDT;

int calculateQuantityOfSlaveProcessesToCreate(int quantityOfFiles);
void createSlaveProcesses(slaveADT* slaves, int quantityOfSlaves);
slaveADT createSlave(int writeToSlave, int readFromSlave, int pid);
void terminateSlaves(slaveADT slaves[], int quantityOfSlaves);
int sendFiles(slaveADT slave, int nextfile, char ** files, int quantityOfFiles, int quantityOfFilesToSend);
void send(slaveADT slave, char * file);
int receiveHashes(slaveADT slave, hashedFileADT * hashes, int* sharedMemory, int* position);
int receiveHash(slaveADT slave, hashedFileADT * hashes, int nextHashedFile,  int* sharedMemory, int* position);
void terminateSlave(slaveADT slave);


#endif
