#include <stdio.h>
#include "slaveProcessLib.h"
#include "testlib.h"
#include "queuelib.h"
#include "hashedfile.h"

void givenAFile();
void givenMultipleFiles();
void givenATaskQueue();

void whenHashMD5IsCalculated();
void whenMultipleFilesAreProcessed();

void thenTheHashMD5IsReturned();
void thenTheHashesAreReturned();

void singleFileHashMD5Test();
void multipleFilesHashMD5Test();
void processTaskQueue();
void receiveFile(char* file);

queueADT taskQueue;
char* filesToUse[] = {"testFile.txt", "testFile1.txt", "testFile2.txt"};
char* testFilePath;

hashedFileADT MD5HashReturned;
char* calculatedMD5Hashes[3];
char* expectedMD5Hashes[] = {"2205e48de5f93c784733ffcca841d2b5",
							 "d41d8cd98f00b204e9800998ecf8427e",
							 "549e80f319af070f8ea8d0f149a149c2"};

int main()
{
	printf("MD5 Hash of one file test: \n");
	singleFileHashMD5Test();

	printf("MD5 Hash of multiple files test: \n");
	multipleFilesHashMD5Test();
}

void singleFileHashMD5Test()
{
	givenAFile();

	whenHashMD5IsCalculated();

	thenTheHashMD5IsReturned();
}

void multipleFilesHashMD5Test()
{
	givenATaskQueue();
	givenMultipleFiles();

	whenMultipleFilesAreProcessed();

	thenTheHashesAreReturned();
}

void givenATaskQueue()
{
	taskQueue = createQueue(3);
}

void givenMultipleFiles()
{
	int i;
	for(i = 0; i < 3; i++)
	{
		receiveFile(filesToUse[i]);
	}
}

void receiveFile(char* file)
{
	enqueueElement(taskQueue, file);
}

void givenAFile()
{
	testFilePath = filesToUse[0];
}

void whenHashMD5IsCalculated()
{
	MD5HashReturned = calculateFileMD5Hash(testFilePath);
}

void whenMultipleFilesAreProcessed()
{
	processTaskQueue();
}

void processTaskQueue()
{
	char* file;
	hashedFileADT hashMD5Calculated;
	int i = 0;

	while(!isEmpty(taskQueue))
	{
		file = dequeueElement(taskQueue);
		hashMD5Calculated = calculateFileMD5Hash(file);
		calculatedMD5Hashes[i] = hashMD5Calculated->hash;
		i++;
	}
}

void thenTheHashMD5IsReturned()
{
	if(compareWithExpectedMD5Hash(expectedMD5Hashes[0], MD5HashReturned->hash) == 0)
	{
		ok();
	}
	else
	{
		fail("Hash obtained is different from Hash expected\n");
	}
}

void thenTheHashesAreReturned()
{
	int i;

	for(i = 0; i < 3; i++)
	{
		if(compareWithExpectedMD5Hash(expectedMD5Hashes[i], calculatedMD5Hashes[i]) != 0)
		{
			fail("There is a different hash \n");
			return;
		}
	}
	ok();
}
