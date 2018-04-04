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
  int sharedMemoryId;
  int semaphoreId;
  int* sharedMemoryAddress;
  key_t key;
  int appProcessPid = atoi(argv[1]);
  printf("%d\n", appProcessPid);

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
  accessSharedMemory(semaphoreId);
  i = 1;
  while (i < sharedMemoryAddress[0])
  {
    fwrite(sharedMemoryAddress+i, 1, 1, stdout);
    i++;
  }
  leaveSharedMemory(semaphoreId);
  shmdt(sharedMemoryAddress);
}
