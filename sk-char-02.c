/* This is Scull device driver program 
 * This is good example for basic character driver *
 *
 *  mknod /dev/testchar c 300 0
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>  /* this is for user_put() */
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antony Herald ");
MODULE_DESCRIPTION("This dummy char driver");

#define MINORNO 0
#define SUCCESS 0
#define DEV_NAME "ANTO_Drv"
#define BUFFER  64

//static dev_t  first;  //Global var for device number (this will hold MAJOR(12) & MINOR(20) numbers)
//static struct cdev charDev; //Global var for char devic
//static struct class * cl;   // Global var for device class

static dev_t  first;  //first is the begining dev number (this will hold MAJOR(12) & MINOR(20) numbers)
static struct class * cl;   // Global var for device class
static struct cdev charDev; //Global var for char drevice
int count = 1;  // support for number minor number 
static char c='Z';

//static char mesg[BUFFER];
//static char *msg_ptr;
static int d_open(struct inode *i, struct file *f)
{
       printk(KERN_INFO " Driver Open() called \n");
       return 0;
}

static int d_release(struct inode *i, struct file *f)
{
       printk(KERN_INFO " Driver Close() called \n");
       return 0;
}
       
static ssize_t d_read(struct file *f, const char __user *ubuf, size_t len, loff_t *off)
{
       printk(KERN_INFO " Driver Read() called \n");

       if (copy_to_user(ubuf, &c, 1) != 0 )
	   return -EFAULT;  
 
       return 1;
}
       
static ssize_t d_write(struct file *f, const char __user *ubuf, size_t len, loff_t *off)
{
       printk(KERN_INFO " Driver Write() called \n");
       if (copy_from_user(&c, ubuf + len -1, 1) != 0 )
	   return -EFAULT; 
       return len;
}

/* fops declare */
static struct file_operations fops = {
     .owner = THIS_MODULE,
     .open  = d_open,
     .release = d_release,
     .read  = d_read,
     .write = d_write
};


static int __init char_dev_init(void)
{
  int ret;
  //first = MKDEV(MAJORNO, MINORNO);
  //register_chrdev_region(MajorNumber, count,DEV_NAME); 
  // 1. Allocation device region
  // 2. create device class
  // 3. create device node automatically
  // 4. Initialize device
  // 5. Add device 

 // 1. Allocation device region
 // DEV_NAME "AntoDevice" this will be created in /proc/devices
 //if(alloc_chrdev_region(&first, 0, count, DEV_NAME) < 0)
 if(alloc_chrdev_region(&first, 0, count, "AntoDevice") < 0)
   {
     printk(KERN_ALERT "register chardrv - failed \n");
     return -1 ;
   }
   printk(KERN_INFO "<Major, Minor: %d, %d >\n", MAJOR(first), MINOR(first)); 

  
 //2. create device class
 //This will be class created in /sys/class/AntoDeviceClass
  if (( cl = class_create(THIS_MODULE, "AntoDeviceClass")) == NULL)
  {
     unregister_chrdev_region(first, count);
     return -1; 
  }

  //3. create device node automatically ie., /dev/AntoCharDevice is created
  if ( device_create(cl, NULL, first, NULL, "AntoCharDevice") == NULL)
  {
     class_destroy(cl);
     unregister_chrdev_region(first, count);
     return -1; 
  }

  //4. initialize device (device register/alloc, created device class, created deviceNote,
  //now initialize ) 
    cdev_init(&charDev, &fops);

  //5.Add device to the kernel 
  ret = cdev_add(&charDev, first, count);
  if (ret < 0 ){
    printk(KERN_ALERT "register charDevice - failed %d \n", ret);

    class_destroy(cl);
    device_destroy(cl,first);
    unregister_chrdev_region(first, count);
  }
  return SUCCESS;
}

static void __exit char_dev_exit(void)
{
       	//int ret;
	device_destroy(cl,first);
	class_destroy(cl);
	cdev_del(&charDev);
	unregister_chrdev_region(first, count);
	printk(KERN_ALERT "Exit Module \n");
   
}

module_init(char_dev_init);
module_exit(char_dev_exit);
