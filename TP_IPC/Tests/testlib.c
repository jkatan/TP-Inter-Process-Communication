#include <stdio.h>
#include <string.h>
#include "testlib.h"

int compareWithExpectedMD5Hash(char* expectedMD5Hash, char* MD5HashToCompare)
{
	return strcmp(expectedMD5Hash, MD5HashToCompare);
}

void ok()
{
	printf("Ok!\n");
}

void fail(char* errorMessage)
{
	printf("%s\n", errorMessage);
}
