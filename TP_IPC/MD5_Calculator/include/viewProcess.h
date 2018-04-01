#ifndef VIEW_PROCESS_LIB_H
#define VIEW_PROCESS_LIB_H

void executeMD5HashCommand(char* fileName);
void linkWriteEndOfPipeWithSTDOUT(int fileDescriptors[]);
void linkReadEndOfPipeWithSTDIN(int fileDescriptors[]);

#endif
