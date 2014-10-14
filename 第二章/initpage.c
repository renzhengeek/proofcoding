/*
 * =====================================================================================
 *
 *       Filename:  initpage.c
 *
 *    Description:  模型初始化页表
 *
 *        Version:  1.0
 *        Created:  2014年10月14日 14时34分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jeff (jeff), zyfforlinux@163.com
 *        Company:  xupt
 *
 * =====================================================================================
 */


#define NR_PGT 0x4  //页表的个数
#define PGD_BASE (unsigned int*)0x1000 //页目录的基地址，从内存的第二个页开始,第一个页的地址是0x0000
#define PAGE_OFFSET (unsigned int)0x2000 //页表的基地址,从内存的第三个页开始,第二页的地址是0x1000

#define PTE_PRE 0x01 //页目录和页表的页目录项和页表项的的位掩码,最低位代表表示这个页/页表是否在内存中
#define PTE_RW 0x02  //倒数第二个位代表这个页/页表的读写权限，只读为１，只写为０
#define PTE_USR 0x04 //倒数第三个位代表这个是否采用写透模式(既写内存，也写高速缓存)
//还有一些其它的位这里没有去定义，默认都是０

void page_init()
{
	int pages = NR_PGT;
	
	unsigned int page_offset = PAGE_OFFSET;
	unsigned int *pgd = PGD_BASE; 	//页目录表位于物理内存的第二个页框内
	unsigned int phy_add = 0x0000;	//在物理地址的最低端建立页机制所需的表格,表示从物理地址的什么地址开始内存映射
	//页表从物理内存的第三个页框处开始
	unsigned int * pgt_entry = (unsigned int *)0x20000; //pgt_entry指向页表的起始地址

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
