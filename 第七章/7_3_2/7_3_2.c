#include <linux/init.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <asm/atomic.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define PRODUCT_NUMS 10

static struct semaphore sem_producer;
static struct semaphore sem_consumer;
static char product[12];

static atomic_t num;

static int producer(void* product);
static int consumer(void* product);

static struct task_struct* producer_tsk;
static struct task_struct* consumer_tsk;

static int id = 1;
static int consume_num = 1;

static int producer(void* p)
{
	char* product = (char*)p;
	int i;

	atomic_inc(&num);	//atomic increase +1
	printk("producer[%d] start...\n", current->pid);

	for(i = 0; i<PRODUCT_NUMS; i++){
		down(&sem_producer);
		snprintf(product, 12, "2014-01-%d", id++);
		printk("producer[%d] produce%s\n", current->pid, product);
		up(&sem_consumer);
	}
	printk("producer[%d] exit...\n", current->pid);
	return 0;
}

static int consumer(void* p)
{
	char* product = (char*)p;

	printk("consumer[%d] start...\n", current->pid);

	for(;;){
		msleep(100);
		down_interruptible(&sem_consumer);
		if(consume_num >= PRODUCT_NUMS * atomic_read(&num))
			break;
		printk("consumer[%d] consume%s\n", current->pid, product);
		consume_num++;
		memset(product, '\0', 12);
		up(&sem_producer);
	}
	printk("consumer[%d] exit...\n", current->pid);
	return 0;
}

static __init int procon_init(void)
{
	printk(KERN_INFO"show producer and consumer\n");
	
	/**
	* init_MUTEX was replaced by:
	* sema_init(struct semaphore* sem, int val)
	* after 2.6
	*
	* static inline void init_MUTEX(struct semaphore* sem)
	 {
	  		sema_init(sem, 1);
	 }
	* static inline void init_MUTEX_LOCKED(struct semaphore* sem)
	 {
			sema_init(sem, 0);
	 }

	* before 2.6 you can do it like this:
	init_MUTEX(&sem_producer);
	init_MUTEX_LOCKED(&sem_consumer);

	* but you need to do it like this in the latest kernel version:
	* for me, my kernel ver is 3.17
	*/

	sema_init(&sem_producer, 1);	// init_MUTEX
	sema_init(&sem_consumer, 0);	// init_MUTEX_LOCKED

	atomic_set(&num, 0);

	/**
	* kernel_thread located in kernel/fork.c
	* kernel_thread may not be used in start a new kernel thread by a kernel module
	* I just got a :
	* insmod: ERROR: could not insert module cumr_prdc.ko: Unknown symbol in module
	* while I tried to insmod this
	kernel_thread(producer, product, CLONE_SIGHAND);
	kernel_thread(consumer, product, CLONE_SIGHAND);
	* 
	* so I used kthread_run func instead
	*/

	producer_tsk = kthread_run(producer, product, "producer kthread");
	consumer_tsk = kthread_run(consumer, product, "consumer kthread");


	return 0;
}

static __exit void procon_exit(void)
{
	printk(KERN_INFO"exit producer and consumer\n");
}

module_init(procon_init);
module_exit(procon_exit);

MODULE_LICENSE("Dual BSD/GPL");