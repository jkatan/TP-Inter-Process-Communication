#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/sem.h>
#define  SEMAPHORE_IDENTIFIER 131313

typedef struct semaphoreCDT* semaphoreADT;

typedef struct semaphoreCDT
{
	char* filename;
	char* hash;
} semaphoreCDT;


#endif
