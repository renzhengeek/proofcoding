#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define N 10

void main()
{
	int i, sum = 0, fd;
	int *result_ptr = mmap(0, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	
	int pid = fork();
	if (pid == 0)       /*子进程，进行计算*/
	{
		for(i=1; i<=N; i++)
		{
			sum += i;
		}
		*result_ptr = sum;
	}else{             /*父进程，等待计算结果*/
		wait(0);
		printf("result = %d\n", *result_ptr);
	}
}
