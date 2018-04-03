#include <stdlib.h>
#include <string.h>
#include "hashedFile.h"

hashedFileADT createHashedFile(char* filename)
{
	hashedFileADT newHashedFile = (hashedFileADT) malloc(sizeof(hashedFileCDT));
	newHashedFile->filename = calloc( 1, (strlen(filename)+ 1) * sizeof(char));
	newHashedFile->hash = calloc(1, sizeof(char) * (HASH_LENGTH + 1));
	return newHashedFile;
}
