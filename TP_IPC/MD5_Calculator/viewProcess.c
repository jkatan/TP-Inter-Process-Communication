#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>

#include  <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "include/hashedfile.h"
#include "include/viewProcess.h"

hashedFileCDT * initSharedMemory(void) {

  int       sharedMemoryId;
  key_t     memoryKey;
  int       *sharedMemoryPointer;

  memoryKey = ftok(".", 'a');
  sharedMemoryId = shmget(memoryKey, 1024*sizeof(hashedFileCDT), 0666);
  if (sharedMemoryId < 0) {
       printf("*** shmget error (client) ***\n");
       exit(1);
  }

  sharedMemoryPointer = shmat(sharedMemoryId, NULL, 0);
  if ((int) sharedMemoryPointer == -1) { /* attach */
       printf("*** shmat error (client) ***\n");
       exit(1);
  }

  return (hashedFileCDT *)sharedMemoryPointer;

}

int main (int argc, char *argv[]){

    //  pid_t appProcessPid = atoi(argv[1]);

    //  sem_t *semaphore = sem_open("/tp_semaphore", O_RDWR, 1);

    sem_t *semaphore = sem_open ("/tp_semaphore", O_CREAT | O_EXCL, 0644, 1);
    sem_unlink ("/tp_semaphore");


    hashedFileCDT * sharedArray = initSharedMemory();

    printf("%s",sharedArray[0].filename);

     int i = 0;
     while (1) {
          sem_wait(semaphore);
          if (sharedArray[i].hash != NULL) {
            printf("<%s>:<%s>\n",sharedArray[i].filename, sharedArray[i].hash);
            i++;
          }
          sem_post(semaphore);
     }
}
