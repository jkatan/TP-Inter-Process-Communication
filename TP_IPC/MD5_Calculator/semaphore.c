#include <stdlib.h>
#include "semaphore.h"

union semun * createSemun()
{
	union semun * newsemun = malloc(sizeof(union semun));
	newsemun->val = 1;
	return newsemun;
}
