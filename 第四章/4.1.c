#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int i;
	unsigned char * buff;
	buff = (char *)malloc(sizeof(char) * 1024);
	printf("My pid is:%d\n", getpid());
	for(i=0; i<60; i++)
	{
		sleep(60);
	}
	return 0;
}
