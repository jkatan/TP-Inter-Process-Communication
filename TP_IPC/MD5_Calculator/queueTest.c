#include <stdio.h>
#include <string.h>
#include "queuelib.h"
#include "testLib.h"

void givenAQueue();
void givenAnElement();
void givenAQueueWithOneElement();
void givenAQueueWithTwoElements();

void whenElementIsDequed();
void whenElementIsEnqueued();

void thenNullIsReturned();
void thenSizeIsEqualToOne();
void thenSizeIsEqualToZero();
void thenSizeIsEqualToOne();
void thenTheFirstElementIsReturned();

void emptyQueueTest();
void enqueueElementTest();
void dequeueElementTest();
void dequeueOneOutOfTwoElementsTest();

queueADT queueToTest;
char* elementToDeque;
char* elementToEnqueue;
char* elementDequed;

int testQueueSize = 3;
char* elementToEnqueueValue = "TestElement";
char* secondElementToEnqueueValue = "TestElement2";

int main()
{
	printf("Empty queue test\n");
	emptyQueueTest();

	printf("Enqueue element test\n");
	enqueueElementTest();

	printf("Dequeue element test\n");
	dequeueElementTest();

	printf("Dequeue one out of two element test\n");
	dequeueOneOutOfTwoElementsTest();
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

void dequeueOneOutOfTwoElementsTest()
{
	givenAQueueWithTwoElements();

	whenElementIsDequed();

	thenTheFirstElementIsReturned();
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

void givenAQueueWithTwoElements()
{
	queueToTest = createQueue(testQueueSize);
	enqueueElement(queueToTest, elementToEnqueueValue);
	enqueueElement(queueToTest, secondElementToEnqueueValue);
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
		fail("Size obtained is not 0 \n");
	}
}

void thenTheFirstElementIsReturned()
{
	if(strcmp(elementToDeque, elementToEnqueueValue) == 0)
	{
		ok();
	}
	else
	{
		fail("Element dequed different from element expected \n");
	}
}