#include <stdio.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	int i;
	int fd[2] = {-1, -1};
	int quantityOfSlaveProcesses = calculateQuantityOfSlaveProcessToCreate(argc);
	createSlaveProcesses(quantityOfSlaveProcesses);
}


int calculateQuantityOfSlaveProcessesToCreate(int quantityOfFiles)
{
	return (int)ceil(quantityOfFiles/5.0);
}

void createSlaveProcesses(int quantityOfSlaveProcesses)
{

}