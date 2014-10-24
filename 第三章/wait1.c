/*
 * =====================================================================================
 *
 *       Filename:  wait1.c
 *
 *    Description:  wait系统调用
 *
 *        Version:  1.0
 *        Created:  2014年10月24日 09时26分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jeff (jeff), zyfforlinux@163.com
 *        Company:  xupt
 *
 * =====================================================================================
 */

#include <stdio.h> //书本中没有加入这个头文件
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

main()
{
	pid_t pc,pr;
	pc = fork();
	if(pc < 0)			/* 如果出错 */
		printf("error ocurred\n");
		else if(pc == 0){	/* 如果是子进程 */
		printf("This is child process with pid of %d\n",getpid());
	sleep(10);			/* 睡眠10s */
	}
	else{				/* 如果是父进程 */
		pr = wait(NULL);	/* 在这里等待　*/
		printf("I catched a child process with pid od %d\n",pr);
	}
	exit(0);
}
