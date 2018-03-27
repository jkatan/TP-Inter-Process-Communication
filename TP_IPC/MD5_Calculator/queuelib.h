#ifndef QUEUELIB_H
#define QUEUELIB_H

typedef struct nodeCDT* nodeADT;

typedef struct nodeCDT
{
	char* fileName;
	nodeADT nextNode;

}nodeCDT;

typedef struct queueCDT* queueADT;

typedef struct queueCDT
{
	nodeADT front;
	nodeADT rear;
	int maxSize;
	int actualSize;

}queueCDT;

queueADT createQueue(int maxSize);
void enqueueElement(queueADT myQueue, char* fileToEnque);
char* dequeueElement(queueADT myQueue);
int isEmpty(queueADT myQueue);
int queueCurrentSize(queueADT myQueue);

#endif

