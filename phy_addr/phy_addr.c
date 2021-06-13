#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/cdev.h"
#include "linux/init.h"
#include "linux/fs.h"
#include "linux/slab.h"
#include "linux/uaccess.h"
#include "linux/device.h"
#include "linux/kdev_t.h"
#include "asm/pgtable.h"
#define DEV_NAME "phy_read"
#define MAX_LENGTH 4000

static char *char_device_buf;
struct cdev *cdev;
dev_t dev;
static struct class* dev_class;

char *char_device_rbuf="read operation: sanjam";
static void get_pagetable_macro(void) //printing some macros realted to the page tables
{
	printk("PAGE_OFFSET = 0x%lx\n",PAGE_OFFSET);
	printk("PGDIR_SHIFT = %d\n",PGDIR_SHIFT);
	printk("PUD_SHIFT = %d\n",PUD_SHIFT);
	printk("PMD_SHIFT = %d\n",PMD_SHIFT);
	printk("PAGE_SHIFT = %d\n",PAGE_SHIFT);
	
	printk("PTRS_PER_PGD = %d\n",PTRS_PER_PGD);
	printk("PTRS_PER_PUD = %d\n",PTRS_PER_PUD);
	printk("PTRS_PER_PMD = %d\n",PTRS_PER_PMD);
	printk("PTRS_PER_PTE = %d\n",PTRS_PER_PTE);

	printk("PAGE_MASK = 0x%lx\n",PAGE_MASK);
}
static unsigned long va_to_phy(unsigned long addr) //conversion of address and page table walk
{
	pgd_t *pgd;
	pud_t *pud;
	p4d_t *p4d;
	pmd_t *pmd;
	pte_t *pte;

	unsigned long paddr=0;
	unsigned long page_addr=0;
	unsigned long page_offset=0;

	pgd=pgd_offset(current->mm,addr); //page global directory
	printk("pgd_val = 0x%lx\n",pgd_val(*pgd));
	printk("pgd_index = 0x%lu\n",pgd_index(addr));
	if(pgd_none(*pgd))
	{
		printk(KERN_INFO "not mapped in pgd\n");
		return -1;
	}

	p4d=p4d_offset(pgd,addr); //for 4 level paging 
	printk("p4d_val = 0x%lx\n",p4d_val(*p4d));
	if(p4d_none(*p4d))
	{
		printk(KERN_INFO "not mapped in p4d\n");
		return -1;
	}
	
	pud=pud_offset(p4d,addr); //page upper directory
	printk("pud_val = 0x%lx\n",pud_val(*pud));
	if(pud_none(*pud))
	{
		printk(KERN_INFO "not mapped in pud\n");
		return -1;
	}
	

	pmd=pmd_offset(pud,addr); //page middle directory
	printk("pmd_val = 0x%lx\n",pmd_val(*pmd));
	printk("pmd_index = 0x%lx\n",pmd_index(addr));
	if(pmd_none(*pmd)) 
	{
		printk(KERN_INFO "not mapped in pmd\n");
		return -1;
	}

	pte=pte_offset_kernel(pmd,addr); //page table entry
	printk("pte_val = 0x%lx\n",pte_val(*pte));
	printk("pte_index = %lu\n",pte_index(addr));
	if(pte_none(*pte))
	{
		printk(KERN_INFO "not mapped in pte\n");
		return -1;
	}

	page_addr = pte_val(*pte) & PAGE_MASK; //conversion of address
	page_offset = addr & ~PAGE_MASK;
	paddr = page_addr | page_offset;
	printk("page_addr = %lx, page_offset = %lx\n",page_addr,page_offset);
	printk("virtual_address = %lx, physical_address = %lx\n",addr,paddr);
	
	return paddr;
}
static int char_dev_open(struct inode *inode,struct file *file)
{
	printk(KERN_INFO "open operation invoked\n");
	get_pagetable_macro();
	return 0;
}
static int char_dev_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "releasing");
	return 0;
}
static ssize_t char_dev_write(struct file *file,const char *buf,size_t lbuf, loff_t *ppos)
{
	int nbytes;
	nbytes=copy_from_user(char_device_buf,buf,lbuf);// reading the address from the file provided by the api
	printk(KERN_INFO "recieved from user %s\n",char_device_buf); 
	unsigned long i;
	kstrtoul(char_device_buf,16,&i); //kernel conversion from string to unsigned long
	printk("address is : %lx",i); // just checking here if converted
	va_to_phy(i);
	return nbytes;
}
static ssize_t char_dev_read(struct file *file,char *buf,size_t lbuf,loff_t *ppos) //ignore this as its for reading purposes
{
	int n;
	n=copy_to_user(buf,char_device_rbuf,lbuf);
	printk(KERN_INFO "send from app data %s\n",char_device_rbuf);
	return n;
}
static struct file_operations char_dev_fops={ //file operations
	.owner = THIS_MODULE,
	.write = char_dev_write,
	.read = char_dev_read,
	.open = char_dev_open,
	.release = char_dev_release
};
static int __init  addr_init(void)
{
	int ret,count=1;
	//dynamic allocation of driver with major and minor number
	if(alloc_chrdev_region(&dev,0,count,DEV_NAME)<0){ 
		printk(KERN_ERR "failed to reserve major/minor range\n");
		return -1;
	}
	//allocationg cdev to recog as char driver
	if(!(cdev=cdev_alloc()))
	{
		printk(KERN_ERR "cdev_alloc() failed\n");
		unregister_chrdev_region(dev,count);
		return -1;

	}
	cdev_init(cdev,&char_dev_fops);
	ret=cdev_add(cdev,dev,count);
	if(ret<0)
	{
		printk(KERN_INFO "error registering device driver\n");
		cdev_del(cdev);
		unregister_chrdev_region(dev,count);
		return -1;
	}
	//create a device file and class file in /dev and /sys
	if((dev_class=class_create(THIS_MODULE,DEV_NAME))==NULL)
	{
		printk(KERN_ERR "cannot create the device\n");
		cdev_del(cdev);
		unregister_chrdev_region(dev,count);
		return -1;
	}
	if((device_create(dev_class,NULL,dev,NULL,DEV_NAME))==NULL)
	{
		printk(KERN_ERR "cannot create the device\n");
		class_destroy(dev_class);
		cdev_del(cdev);
		unregister_chrdev_region(dev,count);
		return -1;

	}
	printk(KERN_INFO "device registered %s\n",DEV_NAME);
	printk(KERN_INFO "major number=%d, minor number=%d\n",MAJOR(dev),MINOR(dev));

	char_device_buf=(char *)kmalloc(MAX_LENGTH,GFP_KERNEL);
	return 0;
}

static void __exit addr_exit(void)
{
	cdev_del(cdev);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	kfree(char_device_buf);
	printk(KERN_INFO "Driver unregistered\n");
	return;
}

module_init(addr_init);
module_exit(addr_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SANJAM");
MODULE_DESCRIPTION("PRINT PHYSICAL ADDRESS");
