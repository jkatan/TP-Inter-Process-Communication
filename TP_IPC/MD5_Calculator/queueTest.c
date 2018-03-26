#include <stdio.h>
#include "queuelib.h"
#include "testLib.h"

void givenAQueue();
void givenAnElement();
void givenAQueueWithOneElement();

void whenElementIsDequed();
void whenElementIsEnqueued();

void thenNullIsReturned();
void thenSizeIsEqualToOne();
void thenSizeIsEqualToZero();

void emptyQueueTest();
void enqueueElementTest();
void dequeueElementTest();

queueADT queueToTest;
char* elementToDeque;
char* elementToEnqueue;

int testQueueSize = 3;
char* elementToEnqueueValue = "TestElement";

int main()
{
	printf("Empty queue test\n");
	emptyQueueTest();

	printf("Enqueue element test\n");
	enqueueElementTest();

	printf("Dequeue element test\n");
	dequeueElementTest();
}

void enqueueElementTest()
{
	givenAQueue();
	givenAnElement();

	whenElementIsEnqueued();

	thenSizeIsEqualToOne();
}

void emptyQueueTest()
{
	givenAQueue();

	whenElementIsDequed();

	thenNullIsReturned();
}

void dequeueElementTest()
{
	givenAQueueWithOneElement();

	whenElementIsDequed();

	thenSizeIsEqualToZero();
}

void givenAQueue()
{
	queueToTest = createQueue(testQueueSize);
}

void givenAnElement()
{
	elementToEnqueue = elementToEnqueueValue;
}

void givenAQueueWithOneElement()
{
	queueToTest = createQueue(testQueueSize); //preguntar
	enqueueElement(queueToTest, elementToEnqueue);
}

void whenElementIsDequed()
{
	elementToDeque = dequeueElement(queueToTest);
}

void whenElementIsEnqueued()
{
	enqueueElement(queueToTest, elementToEnqueue);
}

void thenNullIsReturned()
{
	if(elementToDeque == NULL)
	{
		ok();
	}
	else
	{
		fail("Element dequeued expected to be null, but it is not null\n");
	}
}

void thenSizeIsEqualToOne()
{
	if(queueCurrentSize(queueToTest) == 1)
	{
		ok();
	}
	else
	{
		fail("Size obtained is not 1");
	}
}

void thenSizeIsEqualToZero()
{
	if(queueCurrentSize(queueToTest) == 0)
	{
		ok();
	}
	else
	{
		fail("Size obtained is not 0");
	}
}