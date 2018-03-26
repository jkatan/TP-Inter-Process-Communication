#include <stdlib.h>
#include <stdio.h>
#include "queuelib.h"

queue* createQueue(int maxSize)
{
	
	queue* newQueue = (queue*) malloc(sizeof(queue));
	newQueue->front = NULL;
	newQueue->rear = NULL;
	newQueue->maxSize = maxSize;
	newQueue->actualSize = 0;

	return newQueue;
}

int isEmptyTheQueue(queue* myQueue)
{
	return (myQueue->actualSize == 0) ? 1 : 0;
}

void enqueElement(queue* myQueue, char* fileToEnque)
{
	
	if(myQueue->actualSize != myQueue->maxSize)
	{
		node* nodeToAdd = (node*) malloc(sizeof(node));

		nodeToAdd->fileName = fileToEnque;
		nodeToAdd->nextNode = NULL;

		if(isEmptyTheQueue(myQueue))
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

char* dequeElement(queue* myQueue)
{
	if(isEmptyTheQueue(myQueue)){
		perror("The queue is empty");
		return NULL;
	}

	char* fileNameToPop = myQueue->front->fileName;
	myQueue->front = myQueue->front->nextNode;
	myQueue->actualSize--;

	return fileNameToPop;
}
