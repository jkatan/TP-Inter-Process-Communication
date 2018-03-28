#ifndef HASHEDFILE_H
#define HASHEDFILE_H

#define HASH_LENGTH 32

typedef struct hashedFileCDT
{
	char* filename;
	char* hash;
} hashedFileCDT;

typedef struct hashedFileCDT* hashedFileADT;

hashedFileADT createHashedFile(char* filename);

#endif