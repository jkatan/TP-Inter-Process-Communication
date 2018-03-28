#ifndef SLAVE_PROCESS_TEST_H
#define SLAVE_PROCESS_TEST_H

void givenAFile();
void givenMultipleFiles();
void givenATaskQueue();

void whenHashMD5IsCalculated();
void whenMultipleFilesAreProcessed();

void thenTheHashMD5IsReturned();
void thenTheHashesAreReturned();

void singleFileHashMD5Test();
void multipleFilesHashMD5Test();
void processTaskQueue();
void receiveFile(char* file);

#endif
