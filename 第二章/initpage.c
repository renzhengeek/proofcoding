/*
 * =====================================================================================
 *
 *       Filename:  initpage.c
 *
 *    Description:  模型初始化页表
 *
 *        Version:  2.0
 *        Created:  2014年10月24日 14时34分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jeff (jeff), zyfforlinux@163.com
 *        Company:  xupt
 *
 * =====================================================================================
 */

/*
简介:
	从书本中我们知道了什么是分页，什么是二级页表，什么是cr0.cr3寄存器，以及这些寄存器的功能，
但是对于我们来说仅仅是知道了原理要想通过代码来实现这一原理还是很有难处的，所以下面这段代码就是用来实现实现
页表的初始化的．大致描述下过程,这段代码是用来实现二级页表的初始化的，刚开始先设置页目录的基地址和页表的基地址，然后定义
要初始化几个页表，设置页表的大小，并通过循环的去组合权限位和每一个页表的基地址来填充页目录项，最后通过嵌入汇编来开启cr0的分页标志位并将自己定义的页目录
的基地址放到cr3寄存器中．

*/

#define NR_PGT 0x4  //页表的个数
#define PGD_BASE (unsigned int*)0x1000 //页目录的基地址(也称为起始地址)，从内存的第二个页开始,第一个页的地址是0x0000
#define PAGE_OFFSET (unsigned int)0x2000 //页表的偏移地址(也也就是一个页的大小0x2000也就是4K)．

#define PTE_PRE 0x01 //页目录的页目录项和页表的页表项的的位掩码,最低位代表表示这个页/页表是否在内存中
#define PTE_RW 0x02  //倒数第二个位代表这个页/页表的读写权限，只读为１，只写为０
#define PTE_USR 0x04 //倒数第三个位代表这个是否采用写透模式(既写内存，也写高速缓存)
//还有一些其它的位这里没有去定义，默认都是０

void page_init()
{
	int pages = NR_PGT; 
	
	unsigned int page_offset = PAGE_OFFSET;
	unsigned int *pgd = PGD_BASE; 	//页目录表位于物理内存的第二个页框内
	unsigned int phy_addr = 0x0000;	//从物理内存的哪个地址开始进行页表的映射,这里的意思就是从物理内存的0x0000地址处开始做页表映射

	//页表从物理内存的第三个页框处开始,也就是页表的起始地址是内存的0x2000处
	unsigned int * pgt_entry = (unsigned int *)0x2000; //pgt_entry指向页表的起始地址

	while(pages--) //初始化四个页目录项，每一个页目录项指向一个页表的基地址，只有一个页目录表
	{
	//一个页目录项占用４个字节，32位.其中前20位(后12位为0,页表的起始地址的后12位都是0,因为地址都是按页为单位)代表页表的起始地址，后12位可以作为权限位．
		*pgd++ = page_offset|PTE_USR|PTE_RW|PTE_PRE; //设置页目录项
		page_offset += 0x1000; //重新设置下一个页表的起始地址
	}

	pgd = PGD_BASE;//pgd重新指向页目录表的基地址

	//开始映射16M物理内存，初始化页表的页表项
	while(phy_add < 0x1000000){
		*pgt_entry = phy_add|PTE_USR|PTE_RW|PTE_PRE; //设置页表项
		page_add += 0x1000; //重新设置下一个要映射的物理页起始地址
	}

	_asm__volatile_("movl %0,%%cr3;" //%0代表pgd的地址 内联汇编的语法0是占位符是代表传过来的参数,设置页目录地址
			"movl %%cr0,%%eax;" 
			"orl $0x80000000,%%eax;" //开始分页位,将最高位设置为1
			"movl %%eax,%%cr0;"::"r"(pgd):"memory","%eax");
}
