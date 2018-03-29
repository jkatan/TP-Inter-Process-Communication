#ifndef SLAVE_PROCESS_LIB_H
#define SLAVE_PROCESS_LIB_H

#include "hashedfile.h"

hashedFileADT calculateFileMD5Hash(char* fileName);
void executeMD5HashCommand(char* fileName);
void linkWriteEndOfPipeWithSTDOUT(int fileDescriptors[]);
void linkReadEndOfPipeWithSTDIN(int fileDescriptors[]);

#endif