#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "applicationProcessLib.h"
#include "queuelib.h"


int main(int argc, char const* argv[])
{
	char** files = (char **)(argv+1);
	slaveADT* slaves;
	int quantityOfSlaves, maxReadFileDescriptor;
	int* sharedMemoryAddress;
	int sharedMemoryId;
	int semaphoreId;
	key_t key;
	int quantityOfHashesReceived = 0, quantityOfHashesExpected = 0, quantityOfFiles = argc-1;
	int pid = getpid();

	queueADT filesQueue = createQueue(argc);
	enqueueFiles(filesQueue, files, quantityOfFiles);

	quantityOfSlaves = calculateQuantityOfSlaveProcessesToCreate(filesQueue->actualSize);

	printf("Application process starting... \n");

	slaves = malloc(quantityOfSlaves * sizeof(slaveADT));
	createSlaveProcesses(slaves, quantityOfSlaves);
	maxReadFileDescriptor = getMaxReadFileDescriptor(slaves, quantityOfSlaves) + 1;
	/*Creating semaphore*/
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort *array;
  } arguments;
	arguments.val = 1;

	if((key = ftok("./Makefile", pid)) == -1)
	{
		perror("Couldn't create semaphore:");
		terminateSlaves(slaves, quantityOfSlaves);
		exit(1);
	}

	if((semaphoreId = semget(key, 1, IPC_CREAT | 0700)) == -1)
	{
		perror("Couldn't get semaphore");
		terminateSlaves(slaves, quantityOfSlaves);
		exit(1);
	}
	if(semctl(semaphoreId, 0, SETVAL, arguments) == -1)
	{
		perror("Couldn't init semaphore:");
		terminateSlaves(slaves, quantityOfSlaves);
		exit(1);
	}

	/*Get shared Memory*/
	if((sharedMemoryId = shmget(key, SHARED_MEMORY_SIZE, 0777 | IPC_CREAT)) == -1)
	{
    perror("Couldn't create shared memory");
		terminateSlaves(slaves, quantityOfSlaves);
    exit(1);
  }
	if((sharedMemoryAddress = shmat(sharedMemoryId, (void *)0, 0)) == (int*)-1)
	{
		perror("Couldn't map memory");
		terminateSlaves(slaves, quantityOfSlaves);
		exit(1);
	}

	accessSharedMemory(semaphoreId);
	sharedMemoryAddress[0] = FIRST_POSITION_TO_WRITE;
	sharedMemoryAddress[1] = VIEW_PROCESS_START_FLAG;
	leaveSharedMemory(semaphoreId);

	/*Processing files*/
	quantityOfHashesExpected = filesQueue->actualSize;
	while(quantityOfHashesReceived < quantityOfHashesExpected)
	{
		sendFiles(slaves, quantityOfSlaves, filesQueue);
		quantityOfHashesReceived += receiveHashes(slaves, quantityOfSlaves, sharedMemoryAddress, maxReadFileDescriptor, semaphoreId);
	}

	/*End Process*/
	terminateSlaves(slaves, quantityOfSlaves);
	free(slaves);
	freeQueue(filesQueue);

	accessSharedMemory(semaphoreId);
	sharedMemoryAddress[1] = VIEW_PROCESS_END_FLAG;
	leaveSharedMemory(semaphoreId);

	sendSharedMemoryDataToNewFile("SavedHashes.txt", sharedMemoryAddress);

	semctl(semaphoreId, 0, IPC_RMID, arguments);
	shmdt(sharedMemoryAddress);
	shmctl(sharedMemoryId, IPC_RMID, NULL);

	printf("Application process ending...\n");

	return 0;
}
