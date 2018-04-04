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
	int pid = getpid();
	int* sharedMemoryAddress;
	int position = 1;
	int maxReadFileDescriptor;
	int sharedMemoryId;
	int semaphoreId;
	key_t key;



	char ** files = (char **)(argv+1);
	int quantityOfFiles = argc - 1;
	int quantityOfHashesReceived = 0;
	int nextFile = 0;
	printf("Application process starting... \n");
	printf("PID of application process: %d \n", getpid());
	int quantityOfSlaves = calculateQuantityOfSlaveProcessesToCreate(quantityOfFiles);
	printf("%d slave(s) created...\n", quantityOfSlaves);


	slaveADT* slaves = malloc(quantityOfSlaves * sizeof(slaveADT)); // preguntar
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


	if((key = ftok("./memory", pid)) == -1)
	{
		perror("Couldn't create semaphore:");
		exit(1);
	}

	if((semaphoreId = semget(key, 1, IPC_CREAT | 0600)) == -1)
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
	if((sharedMemoryId = shmget(key, SHARED_MEMORY_SIZE, 0777 | IPC_CREAT)) == -1)
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
	sharedMemoryAddress[0] = position;
	leaveSharedMemory(semaphoreId);

	/*set select for FileDescriptrs*/
	printf("Program processing... \nTotal quantity of files: %d \n", quantityOfFiles);
	/*Processing files*/
	while(quantityOfHashesReceived < quantityOfFiles)
	{
			if(nextFile < quantityOfFiles)
				nextFile = sendFiles(slaves, quantityOfSlaves, files, quantityOfFiles, nextFile);
			accessSharedMemory(semaphoreId);
			quantityOfHashesReceived += receiveHashes(slaves, quantityOfSlaves, sharedMemoryAddress, maxReadFileDescriptor);
			leaveSharedMemory(semaphoreId);
	}

	printf("Quantity of hashes received:%d \n", quantityOfHashesReceived);
	sleep(120);
	/*End Process*/
	printf("Program ending... \n");
	terminateSlaves(slaves, quantityOfSlaves); //chequear si está liberando bien la memoria.
	free(slaves);

	semctl(semaphoreId, 0, IPC_RMID, arguments);
	shmdt(sharedMemoryAddress);
	shmctl(sharedMemoryId, IPC_RMID, NULL);

	return 0;
}
