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
int sendFiles(slaveADT* slaves, int quantityOfSlaves, char** files, int quantityOfFiles, int nextFile);
void send(slaveADT slave, char* file);
int receiveHashes(slaveADT* slaves,  int quantityOfSlaves, int* sharedMemoryAddress, int maxReadFileDescriptor);
int receiveHash(slaveADT slave,  int* sharedMemory);
void terminateSlave(slaveADT slave);
int getMaxReadFileDescriptor(slaveADT* slaves, int quantityOfSlaves);


#endif
