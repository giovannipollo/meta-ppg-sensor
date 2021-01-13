#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>


#include "data.h"

static dev_t mymod_dev;

struct cdev mymod_cdev;

struct class *myclass = NULL;

static char buffer[64];

static int i = 0;

// int numero = 1;

int letto[1];

ssize_t mymod_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	
	letto[0] = ppg[i];
	i++;
	copy_to_user(buf, letto, count);
	if(i == 2048)
		i = 0;
	// printk(KERN_INFO "[mymod] read (count=%d, offset=%d)\n", (int)count, (int)*f_pos );
	return count;
}

ssize_t mymod_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	/* int scritto[1];
    	if (copy_from_user(scritto, buf, count))
        	return -EFAULT;
    	printk(KERN_INFO "The value copied from user =   %d", scritto[0]);
   	return sizeof(buf);
*/
	//printk(KERN_INFO "[mymod] write (count=%d, offset=%d)\n", (int)count, (int)*f_pos );

	//return count;
}


ssize_t mymod_open(struct inode *inode_temp, struct file *filp)
{
	printk(KERN_INFO "[mymod] open");
	i = 0;
	return 0;
}

ssize_t mymod_release(struct inode *inode_temp, struct file *filp)
{
	printk(KERN_INFO "[mymod] release");

	return 0;
}


struct file_operations mymod_fops = {
	.owner = THIS_MODULE,
	.read = mymod_read,
	.write = mymod_write,
	.open = mymod_open,
	.release = mymod_release,
};

static int __init mymod_module_init(void)
{
	printk(KERN_INFO "Loading mymod_module\n");

	alloc_chrdev_region(&mymod_dev, 0, 1, "mymod_dev");
	printk(KERN_INFO "%s\n", format_dev_t(buffer, mymod_dev));

	myclass = class_create(THIS_MODULE, "mymod_sys");
	device_create(myclass, NULL, mymod_dev, NULL, "mymod_dev");	
	
	cdev_init(&mymod_cdev, &mymod_fops);
	mymod_cdev.owner = THIS_MODULE;
	cdev_add(&mymod_cdev, mymod_dev, 1);

	return 0;
}

static void __exit mymod_module_cleanup(void)
{
	printk(KERN_INFO "Cleaning-up mymod_dev.\n");

	cdev_del(&mymod_cdev);
	unregister_chrdev_region(mymod_dev, 1);
}

module_init(mymod_module_init);
module_exit(mymod_module_cleanup);

MODULE_AUTHOR("MassimoViolante");
MODULE_LICENSE("GPL");
