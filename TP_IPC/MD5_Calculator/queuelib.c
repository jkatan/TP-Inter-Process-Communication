#include <stdlib.h>
#include <stdio.h>
#include "queuelib.h"

queueADT createQueue(int maxSize)
{
	
	queueADT newQueue = (queueADT) malloc(sizeof(queueCDT));
	newQueue->front = (nodeADT) NULL;
	newQueue->rear = (nodeADT) NULL;
	newQueue->maxSize = maxSize;
	newQueue->actualSize = 0;

	return newQueue;
}

int queueCurrentSize(queueADT myQueue)
{
	return myQueue->actualSize;
}

int isEmpty(queueADT myQueue)
{
	return (myQueue->actualSize == 0) ? 1 : 0;
}

void enqueueElement(queueADT myQueue, char* fileToEnque)
{
	
	if(myQueue->actualSize != myQueue->maxSize)
	{
		nodeADT nodeToAdd = (nodeADT) malloc(sizeof(nodeCDT));

		nodeToAdd->fileName = fileToEnque;
		nodeToAdd->nextNode = NULL;

		if(isEmpty(myQueue))
		{
			myQueue->front = nodeToAdd;
			
		}
		else
		{
			myQueue->rear->nextNode = nodeToAdd;
		}

		myQueue->actualSize++;
		myQueue->rear = nodeToAdd;
	}
	else
	{
		perror("The queue is full");
	}
	return;
}

char* dequeueElement(queueADT myQueue)
{
	if(isEmpty(myQueue)){
		perror("The queue is empty");
		return NULL;
	}

	char* fileNameToPop = myQueue->front->fileName;
	myQueue->front = myQueue->front->nextNode;
	myQueue->actualSize--;

	return fileNameToPop;
}
