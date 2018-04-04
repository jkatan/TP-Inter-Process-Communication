#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define SHARED_MEMORY_SIZE (4096+34)*sizeof(int)

void accessSharedMemory(int semphoreId);
void leaveSharedMemory(int semphoreId);

#endif
