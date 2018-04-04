#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include  <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sharedMemory.h"

int main (int argc, char *argv[])
{
  int i, terminate;
  int sharedMemoryId, semaphoreId;
  int* sharedMemoryAddress;
  key_t key;
  int appProcessPid; 

  if(argc < 2)
  {
    printf("No arguments passed to program, viewProcess receives one argument\n");
    exit(0);
  }

  key = ftok("./Makefile", appProcessPid);

  /*Get semaphore*/
  if((semaphoreId = semget(key, 1, IPC_CREAT | 0600)) == -1)
  {
    perror("Couldn't get semaphore");
    exit(1);
  }

  /*Get shared Memory*/
  if((sharedMemoryId = shmget(key, SHARED_MEMORY_SIZE, 0777 | IPC_CREAT)) == -1)
  {
    perror("Couldn't create shared memory");
    exit(1);
  }
  if((int)(sharedMemoryAddress = shmat(sharedMemoryId, (void *)0, 0)) == -1)
  {

    perror("Couldn't map memory");
    exit(1);
  }
  i = 2;
  terminate = 0;
  while(!terminate)
  {
    accessSharedMemory(semaphoreId);
    while (i < sharedMemoryAddress[0])
    {
      fwrite(sharedMemoryAddress+i, 1, 1, stdout);
      i++;
    }
    terminate = sharedMemoryAddress[1];
    leaveSharedMemory(semaphoreId);
  }
  shmdt(sharedMemoryAddress);
}
