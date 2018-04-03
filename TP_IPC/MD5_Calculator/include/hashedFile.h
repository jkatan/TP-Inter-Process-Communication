#ifndef HASHEDFILE_H
#define HASHEDFILE_H

#define HASH_LENGTH 32

typedef struct hashedFileCDT* hashedFileADT;

typedef struct hashedFileCDT
{
	char* filename;
	char* hash;
} hashedFileCDT;



hashedFileADT createHashedFile(char* filename);

#endif
