// Copyright (c) 2014, ZhaoStudio
// Author: CptZhao<zhaonanyu@gmail.com>
// Created: 2014-10-24
// Description:
// ISR
#include<linux/module.h>
#include<linux/init.h>
#include<linux/interrupt.h>
#include<linux/kernel.h>

static int irq;
static char* interface;
static int count = 0;

module_param(interface, charp, 0644);
module_param(irq, int , 0644);

static irqreturn_t intr_handler(int irq, void* dev_id)
{
	static long interval = 0;

	if(count == 0){
		interval = jiffies;
	}

	interval = jiffies - interval;
	printk("The interval between two interrupts is %ld\n", interval);
	interval = jiffies;
	count++;

	return IRQ_NONE;
}

static int __init intr_init(void)
{
	if(request_irq(irq, &intr_handler, IRQF_SHARED, interface, &irq))
	{
		printk(KERN_ERR "Fails to register IRQ %d\n", irq);
		return -EIO;
	}
	printk("%s Request on IRQ %d succeeded\n", interface, irq);
	return 0;
}

static void __exit intr_exit(void)
{
	printk("The %d interrupts happened on irq %d", count, irq);
	free_irq(irq, &irq);
	printk("Freeing IRQ %d\n", irq);
	return;
}

module_init(intr_init);
module_exit(intr_exit);

MODULE_LICENSE("GPL");
