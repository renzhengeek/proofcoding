/*`
 * =====================================================================================
 *
 *       Filename:  1-6-3.c
 *
 *    Description:  链表的应用
 *
 *        Version:  1.0
 *        Created:  2014年10月09日 17时16分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jeff (jeff), zyfforlinux@163.com
 *        Company:  xupt
 *
 * =====================================================================================
 */


#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/list.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("XIYOU");

#define N 10 			//链表的节点数

struct numlist {
	int num; 		//数据
	struct list_head list;  //指向双链表前后结点的指针
};

struct numlist numhead; 	//头结点

static int __init doublelist_init(void)
{
	//初始化头结点
	struct numlist *listnode;
	struct list_head *pos;
	struct numlist *p;
	int i;
	
	printk("doublelist is staring...\n");
	INIT_LIST_HEAD(&numhead.list);
	//建立N个结点，依次加入到链表当中
	for(i = 0;i<N;i++){
		listnode = (struct numlist *)kmalloc(sizeof(struct numlist),GFP_KERNEL); //kmalloc 在内核空间申请内存，类似于malloc(),参见第4章
		listnode->num = i+1;
		list_add_tail(&listnode->list,&numhead.list); //尾插法
		printk("NOde %d has added to the doublelist..\n",i+1);
	}
	//遍历链表
	i = 1;
	list_for_each(pos,&numhead.list){
		p = list_entry(pos,struct numlist,list);
		printk("Node %d's data:%d\n",i,p->num);
		i++;
	}
	
	return 0;
}


static void __exit doublelist_exit(void)
{
	struct list_head *pos,*n;
	struct numlist *p;
	int i;
	
	//依次删除N个结点
	i = 1;
	list_for_each_safe(pos,n,&numhead.list){				//安全的删除结点而进行遍历，用n占存下一个结点。
		list_del(pos);							//从双链表在中删除当前结点
		p = list_entry(pos,struct numlist,list);			//得到当前数据结点的首地址，即指针。
		kfree(p);							//释放数据结点所占空间
		printk("Node %d has removed from the doublelist...\n",i++);
	}
	printk("doublelist is exiting..\n");
}


module_init(doublelist_init);
module_exit(doublelist_exit);
