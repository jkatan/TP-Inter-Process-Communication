#ifndef QUEUELIB_H
#define QUEUELIB_H value


typedef struct node node;

typedef struct node
{

	char* fileName;
	node* nextNode;

}node;

typedef struct queue
{

	node* front;
	node* rear;
	int maxSize;
	int actualSize;

}queue;

queue* createQueue(int maxSize);
void enqueElement(queue* myQueue, char* fileToEnque);
char* dequeElement(queue* myQueue);
int isEmptyTheQueue(queue* myQueue);

#endif

