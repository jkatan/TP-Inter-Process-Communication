#include <stdio.h>
#include "slaveProcess.h"
#include "testLib.h"

void givenAFile();

void whenHashMD5IsCalculated();

void thenTheHashMD5IsReturned();

void singleFileHashMD5Test();

char *testFilePath;
char *fileToUse = "TestFile.txt";

char *MD5HashReturned;

char* expectedMD5Hash = "2205e48de5f93c784733ffcca841d2b5";

int main()
{
	printf("MD5 Hash of one file test: \n");
	singleFileHashMD5Test();
}

void singleFileHashMD5Test()
{
	givenAFile();

	whenHashMD5IsCalculated();

	thenTheHashMD5IsReturned();
}

void givenAFile()
{
	testFilePath = fileToUse;
}

void whenHashMD5IsCalculated()
{
	MD5HashReturned = calculateFileMD5Hash(testFilePath);
}

void thenTheHashMD5IsReturned()
{
	if(compareWithExpectedMD5Hash(expectedMD5Hash, MD5HashReturned) == 0)
	{
		ok();
	}
	else
	{
		fail("Hash obtained is different from Hash expected\n");
	}
}