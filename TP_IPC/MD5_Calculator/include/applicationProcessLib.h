#ifndef APPLICATION_PROCESS_H
#define APPLICATION_PROCESS_H

#define MAX_QTY_FILES 200
#define MIN_QTY_FILES_TO_PROCESS 4
#define QTY_TO_SEND 3
#define FIRST_POSITION_TO_WRITE 2
#define VIEW_PROCESS_START_FLAG 0
#define VIEW_PROCESS_END_FLAG 1
#define SHARED_MEMORY_SIZE (4096+34)*sizeof(int)

#include "hashedFile.h"
#include "semaphore.h"
#include "queuelib.h"

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
void sendFiles(slaveADT* slaves, int quantityOfSlaves, queueADT filesQueue);
void send(slaveADT slave, char* file);
void accessSharedMemory(int semphoreId);
void leaveSharedMemory(int semphoreId);
int receiveHashes(slaveADT* slaves,  int quantityOfSlaves, int* sharedMemoryAddress, int maxReadFileDescriptor, int semaphoreId);
int receiveHash(slaveADT slave,  int* sharedMemory, int semaphoreId);
void terminateSlave(slaveADT slave);
int getMaxReadFileDescriptor(slaveADT* slaves, int quantityOfSlaves);
void sendSharedMemoryDataToNewFile(char* newFileName, int* sharedMemoryAddress);
void enqueueFiles(queueADT myQueue, char* files[], int numberOfFiles);
int isARegularFile(char* path);


#endif
