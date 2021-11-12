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
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antony Herald ");
MODULE_DESCRIPTION("This dummy char driver");

/* function declare */
//static size_t device_read(struct file *, size_t, loff_t *);
//static size_t device_write(struct file *, size_t, loff_t *);
//static int device_open(struct inode*, struct file *);
//static int device_release(struct inode*, struct file *);

/* Defines */
#define MAJORNO 300
#define MINORNO 0
#define SUCCESS 0
#define DEV_NAME "ANTONIO"
#define BUFFER  64

struct cdev * myCdev;
dev_t  mydevice;
int count = 1; 

//static char mesg[BUFFER];
//static char *msg_ptr;

/* fops declare */
static struct file_operations fops = {
     .owner = THIS_MODULE,
//   .read  = device_read,
//   .write = device_write,
//   .open  = device_open,
//   .release = device_release
};


static int __init char_dev_init(void)
{
  
  int ret;

  mydevice = MKDEV(MAJORNO, MINORNO);
  register_chrdev_region(mydevice, count,DEV_NAME); 

  
  myCdev = cdev_alloc();
           cdev_init(myCdev, &fops);
  ret    = cdev_add(myCdev,mydevice, count);

  if (ret < 0 ){
    printk(KERN_ALERT "register chardrv - failed %d \n");
  }
  return SUCCESS;
}

static void __exit char_dev_exit(void)
{
   //int ret;
   cdev_del(myCdev);
   unregister_chrdev_region(mydevice, 1);
   printk(KERN_ALERT "Exit Module \n");
   
}

module_init(char_dev_init);
module_exit(char_dev_exit);
