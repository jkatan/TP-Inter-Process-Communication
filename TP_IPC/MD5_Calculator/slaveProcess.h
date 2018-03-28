#ifndef SLAVE_PROCESS_H
#define SLAVE_PROCESS_

char* calculateFileMD5Hash(char* fileName);
void executeMD5HashCommand(char* fileName);
void linkWriteEndOfPipeWithSTDOUT(int fileDescriptors[]);
void linkReadEndOfPipeWithSTDIN(int fileDescriptors[]);

#endif