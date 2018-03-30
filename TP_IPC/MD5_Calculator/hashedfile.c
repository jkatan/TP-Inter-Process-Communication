#include <stdlib.h>
#include <string.h>
#include "hashedFile.h"

hashedFileADT createHashedFile(char* filename)
{
	hashedFileADT newHashedFile = (hashedFileADT) malloc(sizeof(hashedFileCDT));
	newHashedFile->filename = malloc(strlen(filename)*sizeof(char));
	newHashedFile->hash = malloc(sizeof(char)*HASH_LENGTH);
	return newHashedFile;
}
