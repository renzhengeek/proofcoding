// Copyright (c) 2014, ZhaoStudio
// Author: CptZhao<zhaonanyu@gmail.com>
// Created: 2014-10-29
// Description:
// 调用小任务的相关函数编写一个模块
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>

static struct tasklet_struct my_tasklet;

static void tasklet_handler(unsigned long data)
{
	printk(KERN_ALERT"tasklet_handler is running.\n");
}

static int __init test_init(void)
{
	tasklet_init(&my_tasklet, tasklet_handler, 0);
	tasklet_schedule(&my_tasklet);
	return 0;
}

static void __exit test_exit(void)
{
	tasklet_kill(&my_tasklet);
	printk(KERN_ALERT"test_exit running.\n");
}

MODULE_LICENSE("GPL");

module_init(test_init);
module_exit(test_exit);
