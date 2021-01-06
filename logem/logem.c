/* logem.c - Logger Embedded */ 


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>

#if CONFIG_MODVERSION==1
#define MODVERSION
#include <linux/modversion.h>
#endif

#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "logem.h"

#define DEBUG(x) if (debug>=1) x;


static int debug = 1;
static int device_is_open = 0;
static char message[BUFLEN];
static char *message_pointer;


MODULE_AUTHOR("Nicu Pavel <npavel@easeweb.com>");
MODULE_DESCRIPTION("Kernel module for embedded devices logger");
MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "turns logem module debug off/on (0/1)");
EXPORT_NO_SYMBOLS;

static int device_open(struct inode *inode,struct file *file)
{
    static int counter = 0;
    DEBUG(printk (KERN_INFO "logem: device_open(%p,%p).\n",inode,file));
    DEBUG(printk (KERN_INFO "logem: device: %d.%d\n",
		    inode->i_rdev >> 8, inode->i_rdev & 0xFF));
    if (device_is_open) return -EBUSY;
    if (! (message_pointer = kmalloc(BUFLEN,GFP_KERNEL))) return -ENOMEM;
    device_is_open++;
    sprintf (message,"counter: %d\nmessage: %s\n",counter++,
		    "this is a test message");
    message_pointer = message;
    MOD_INC_USE_COUNT;
    kfree(message_pointer);
    return SUCCESS;

}

static int device_release(struct inode *inode,struct file *file)
{
    DEBUG(printk(KERN_INFO "logem: device_release(%p,%p).\n",inode,file))
    device_is_open--;
    MOD_DEC_USE_COUNT;
    return SUCCESS;
}


static ssize_t device_read(struct file *file,char *buffer,size_t length,loff_t *offset)
{
    int bytes_read = 0;
    if (message_pointer == 0) return SUCCESS;
    while (length && *message_pointer) {
	//copy_to_user (*(message_pointer++),buffer++);
	put_user (*(message_pointer++),buffer++);
	length--;
	bytes_read++;
    }
    DEBUG(printk(KERN_INFO "logem: read %d bytes, %d left\n",bytes_read,length));
    return bytes_read;

}

static ssize_t device_write (struct file *file,const char *buffer,size_t length,loff_t *offset)
{
    int bytes_written;
    DEBUG(printk ("device_write(%p,%s,%d)",file, buffer, length));
    for (bytes_written=0; bytes_written<length && bytes_written<BUFLEN; bytes_written++)
	//copy_from_user(message[bytes_written], buffer+bytes_written);
	get_user(message[bytes_written], buffer+bytes_written);
    message_pointer = message;
    return bytes_written;

}


static int major;
static struct file_operations fops = {
    owner:	THIS_MODULE,
    open:	device_open,
    release:	device_release,
    read:	device_read,
    write:	device_write
};

static int __init logem_init_module(void)
{
    DEBUG(printk (KERN_INFO "logem: begin loading.\n"));
    major = register_chrdev(0,DEVICE_NAME,&fops);
    if ( major < 0 ) {
	printk ("%s device failed with %d\n","logem: registering ",major);
	return major;
    }
    DEBUG(printk (KERN_INFO "logem: module registered with %d major number .\n",major));
    return SUCCESS;
}


static void __exit logem_cleanup_module(void)
{
    int ret;
    DEBUG(printk (KERN_INFO "logem: begin unload.\n"));    
    ret = unregister_chrdev(major,DEVICE_NAME);
    if ( ret < 0 ) printk ("logem: unregister device %d error: %d\n",major,ret);
    else DEBUG(printk (KERN_INFO "logem: succesfully unregister device %d.\n",major));    
}

module_init(logem_init_module);
module_exit(logem_cleanup_module);

