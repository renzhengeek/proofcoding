#include<sys/types.h>  /*提供类型pid_t的定义，在pc上与int型相同*/
#include<unistd.h>     /*提供系统调用的定义*/
#include<stdio.h>      /*提供基本输出输入的函数，如printf*/

void do_something(long t)
{
	int i = 0;
	for(i=0; i<t; i++)
		for(i=0; i<t; i++)
			for(i=0; i<t; i++)
				;
}
int main()
{
	pid_t pid;
	/*此时只有一个进程*/
	printf("PID before fork():%d\n", getpid());
	pid = fork();
	/*此时已经有两个进程在同时在运行*/
	pid_t npid = getpid();
	if(pid < 0)
	{
		perror("fork error\n");
	}
	else if (pid == 0)/*pid == 0表示子进程*/
	{
		while(1)
		{
			printf("I am child process,PID is %d\n", npid);
			do_something(10000000);
		}
	}
	else if(pid >= 0)/*pid > 0 表示父进程*/
	{
		while(1)
		{
			printf("I am father process,PID is %d\n", npid);
			do_something(10000000);
		}
	}
	return 0;
}
