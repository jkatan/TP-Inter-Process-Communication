#ifndef HASHEDFILE_H
#define HASHEDFILE_H

typedef struct hashedfileCDT
{
	int pid;
	char* filename;
	char* hash;
	
} hashedfileCDT;

typedef struct hashedfileCDT* hashedfileADT;