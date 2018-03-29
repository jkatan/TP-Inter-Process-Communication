#include <stdlib.h>
#include <stdio.h>
#include "slaveProcessLib.h"
#include "hashedfile.h"

int main()
{
	char* filename = malloc(sizeof(char)*256);
	hashedFileADT file;

	while(feof(stdin) == 0)
	{
		scanf("%s", filename);
		file = calculateFileMD5Hash(filename);
		printf("%s %s\n", file->hash, file->filename);
	}
}
