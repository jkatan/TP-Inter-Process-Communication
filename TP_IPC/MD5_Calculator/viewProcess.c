#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include  <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "sharedMemory.h"




int main (int argc, char *argv[])
{
  int i;
  int terminate;
  int sharedMemoryId;
  int semaphoreId;
  int* sharedMemoryAddress;
  key_t key;
  int appProcessPid = atoi(argv[1]);
  printf("View process starting... \n");

  key = ftok("./memory", appProcessPid);

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
