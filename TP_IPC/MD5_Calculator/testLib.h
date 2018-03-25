#ifndef TEST_LIB_H
#define TEST_LIB_H

int compareWithExpectedMD5Hash(char* expectedMD5Hash, char* MD5HashToCompare);
void ok();
void fail(char* errorMessage);

#endif