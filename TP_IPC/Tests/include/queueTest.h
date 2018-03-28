#ifndef QUEUE_TEST_H
#define QUEUE_TEST_H

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

#endif
