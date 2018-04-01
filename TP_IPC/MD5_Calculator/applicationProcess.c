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

int main(int argc, char const* argv[])
{
	int i, quantityOfFilesSent, quantityOfHashesReceived;

	int* sharedMemoryAddress;
	int sharedMemoryId;
	int semaphoreId;
	key_t key;

	char ** files = (char **)argv;
	int quantityOfFiles = argc;
	int nextFile = 0;
	hashedFileADT * hashes = NULL;

	int quantityOfSlaves = calculateQuantityOfSlaveProcessesToCreate(quantityOfFiles);

	slaveADT slaves[quantityOfSlaves];
	createSlaveProcesses(slaves, quantityOfSlaves);

	/*Creating semaphore*/
	union semun * arguments = createSemun();

	key = ftok("/semaphore", SEMAPHORE_IDENTIFIER); //check

	if((semaphoreId = semget(key, 1, IPC_CREAT | 0666)) == -1)
	{
		perror("Couldn't get semaphore");
		exit(1);
	}
	if(semctl(semaphoreId, 0, SETVAL, arguments) == -1)
	{
		perror("Couldn't init semaphore");
		exit(1);
	}

	/*Get shared Memory*/
	if((sharedMemoryId = shmget(key, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT)) == -1)
	{
    perror("Couldn't create shared memory");
    exit(1);
  }
	if((int)(sharedMemoryAddress = shmat(sharedMemoryId, (void *)0, 0)) ==-1)
	{
		perror("Couldn't map memory");
		exit(1);
	}

	/*Processing files*/
	while(nextFile < quantityOfFiles)
	{
		for(i = 0; i < quantityOfSlaves; i++)
		{
			accessSharedMemory(semaphoreId);
			quantityOfHashesReceived = receiveHashes(slaves[i], hashes);
			leaveSharedMemory(semaphoreId);
			slaves[i]->filesGivenToProcess -= quantityOfHashesReceived;

			if(slaves[i]->filesGivenToProcess < MIN_QTY_FILES_TO_PROCESS)
			{
				quantityOfFilesSent = sendFiles(slaves[i], nextFile, files, quantityOfFiles, QTY_TO_SEND);
				slaves[i]->filesGivenToProcess += quantityOfFilesSent;
				nextFile += quantityOfFilesSent;
			}
		}
	}

	/*End Process*/
	terminateSlaves(slaves, quantityOfSlaves);
	return 0;
}
