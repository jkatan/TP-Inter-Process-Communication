#ifndef SLAVE_PROCESS_LIB_H
#define SLAVE_PROCESS_LIB_H

#include "hashedFile.h"

hashedFileADT calculateFileMD5Hash(char* fileName);
void executeMD5HashCommand(char* fileName);
void linkWriteEndOfPipeWithSTDOUT(int fileDescriptors[]);
void linkReadEndOfPipeWithSTDIN(int fileDescriptors[]);
void readFileFromPipe(int pipeFileDescriptor, char* fileNameBuffer);
void sendHashedFileThroughPipe(int pipeFileDescriptor, hashedFileADT file);
void terminate(int signum);

#endif
