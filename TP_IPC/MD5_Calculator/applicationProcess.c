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
#include <sys/select.h>
#include "applicationProcessLib.h"




int main(int argc, char const* argv[])
{

	int i, quantityOfFilesSent, quantityOfHashesReceived;
	int pid = getpid();
	int* sharedMemoryAddress;
	int position = 0;
	int maxFileDescriptor = 0;
	int sharedMemoryId;
	int semaphoreId;
	key_t key;
	fd_set fileDescriptorSetToReadFromSlaves;
	fd_set originalFileDescriptorSetToReadFromSlaves;

	char ** files = (char **)argv;
	int quantityOfFiles = argc - 1;
	int nextFile = 0;
	hashedFileADT * hashes = NULL;
	printf("Program starting... \n");
	int quantityOfSlaves = calculateQuantityOfSlaveProcessesToCreate(quantityOfFiles);

	slaveADT slaves[quantityOfSlaves];
	createSlaveProcesses(slaves, quantityOfSlaves);

	/*Creating semaphore*/
	union semun
	{
               int val;
               struct semid_ds *buf;
               ushort *array;
  } arguments;
	arguments.val = 1;


	if((key = ftok("./memory", pid)) == -1)
	{
		perror("Couldn't create semaphore:");
		exit(1);
	}

	if((semaphoreId = semget(key, 1, IPC_CREAT | 0644)) == -1)
	{
		perror("Couldn't get semaphore");
		exit(1);
	}
	if(semctl(semaphoreId, 0, SETVAL, arguments) == -1)
	{
		perror("Couldn't init semaphore:");
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
	accessSharedMemory(semaphoreId);
	sharedMemoryAddress[position]=1;
	position++;
	leaveSharedMemory(semaphoreId);
	printf("Program processing... \nTotal quantity of files: %d \n", quantityOfFiles);
	/*set select for FileDescriptrs*/
	FD_ZERO(&originalFileDescriptorSetToReadFromSlaves);
	for(i = 0; i < quantityOfSlaves; i++)
	{
		if(maxFileDescriptor < slaves[i]->readFrom)
		{
			maxFileDescriptor = slaves[i]->readFrom;
		}
		FD_SET(slaves[i]->readFrom, &originalFileDescriptorSetToReadFromSlaves);
	}
	maxFileDescriptor++;

	/*Processing files*/
	while(nextFile < quantityOfFiles)
	{
		FD_COPY(&originalFileDescriptorSetToReadFromSlaves, &fileDescriptorSetToReadFromSlaves);
		if(select(maxFileDescriptor, &fileDescriptorSetToReadFromSlaves,NULL, NULL, NULL) == -1)
		{
			perror("Error reading file descriptors (select(...))");
			exit(1);
		}
		printf("Quantity of files remaining: %d \n", quantityOfFiles-nextFile);
		for(i = 0; i < quantityOfSlaves; i++)
		{
			accessSharedMemory(semaphoreId);
			printf("Application Process in shared memory... \n");
			if(FD_ISSET(slaves[i]->readFrom, &fileDescriptorSetToReadFromSlaves) != 0)
			{
				quantityOfHashesReceived =  receiveHashes(slaves[i], hashes, sharedMemoryAddress, &position);
				sharedMemoryAddress[0] = position;
			}
			leaveSharedMemory(semaphoreId);
			printf("Out of shared memory... \n");
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
	printf("Program ending... \n");

	semctl(semaphoreId, 0, IPC_RMID, arguments);
	shmdt(sharedMemoryAddress);
	shmctl(sharedMemoryId, IPC_RMID, NULL);

	return 0;
}
