#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define SHARED_MEMORY_SIZE sizeof(int)*4096

void accessSharedMemory(int semphoreId);
void leaveSharedMemory(int semphoreId);

#endif
