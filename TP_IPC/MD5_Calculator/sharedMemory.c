#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "sharedMemory.h"

void accessSharedMemory(int semaphoreId)
{
  struct sembuf semaphoreBuffer;
  semaphoreBuffer.sem_num = 0;
  semaphoreBuffer.sem_op = -1;
  semaphoreBuffer.sem_flg = 0;
  if (semop(semaphoreId, &semaphoreBuffer, 1) == -1)
  {
    perror("Couldn't leave shared memory by changing semaphore");
    exit(1);
  }
}

void leaveSharedMemory(int semaphoreId)
{
  struct sembuf semaphoreBuffer;
  semaphoreBuffer.sem_num = 0;
  semaphoreBuffer.sem_op = 1;
  semaphoreBuffer.sem_flg = 0;
  if (semop(semaphoreId, &semaphoreBuffer, 1) == -1)
  {
    perror("Couldn't leave shared memory by changing semaphore");
    exit(1);
  }
}
