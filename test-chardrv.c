#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#define FIRST_MINOR 0
#define MINOR_CNT 1

static dev_t dev;   //Dev (holds MJR and MNR numbers (12 + 20) 32-bit information
static struct class *cl;
static struct cdev c_dev;

static int my_open(struct inode *i, struct file *f)
{
        printk(KERN_INFO "Driver: Open()\n");  
	return 0;
}
static int my_close(struct inode *i, struct file *f)
{
        printk(KERN_INFO "Driver: Close()\n");  
	return 0;
}

static char c[] = "This is Antonio - 1st Character Driver - data passed from kernel to user SPACE !!!! \0";
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
             printk(KERN_INFO "Driver: read -- 1()\n");  
             if( *off == 0)
             {  
                printk(KERN_INFO "Driver: read -- 2()\n");  
		if (copy_to_user(buf, c, strlen(c)) )   //This is kernel API - copy block of data from kernel space to user space/
		{
			return -EFAULT;
		}
		 *off += 1;
		 return 1;
              }
              else return 0;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	char str[100];
        printk(KERN_INFO "Driver: Write()\n");  
	if (copy_from_user(str, buf + len - 1, 1))
	{
		return -EFAULT;
	}
	printk(KERN_INFO " from UserSpace str  \n  %s ",str); 
	printk(KERN_INFO " from UserSpace buf  \n  %s ",buf); 
	return len;
}

static struct file_operations charDrv_fops =
{
	.owner   = THIS_MODULE,
	.open    = my_open,
	.release = my_close,
	.read    = my_read,
	.write   = my_write
};


static int __init fcd_init(void)
{
	int ret;
	struct device *dev_ret;

	if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "TestFinalDriver")) < 0)
	{
		return ret;
	}
	printk(KERN_INFO "-----------------------------------\n");
	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO "-----------------------------------\n");

	cdev_init(&c_dev, &charDrv_fops);  

	if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
	{
		unregister_chrdev_region(dev, MINOR_CNT);
		return ret;
	}
	
	if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
	{
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, MINOR_CNT);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "TestFinalDr", 0)))
	{
		class_destroy(cl);
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, MINOR_CNT);
		return PTR_ERR(dev_ret);
	}
	return 0;
}

static void __exit fcd_exit(void)
{
	device_destroy(cl, dev);
	class_destroy(cl);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(fcd_init);
module_exit(fcd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("softwaves <Antony Herald> ");
MODULE_DESCRIPTION("A Character Driver");

