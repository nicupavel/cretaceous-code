/* 
    pidwatcher (c) 2001-2006 Ituner Networks Corporation 
    npavel@linuxconsulting.ro
*/

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <sys/syscall.h>

#include <linux/slab.h>
#include <linux/vmalloc.h>

#include "pidwatcher.h"

static int major;
/* This is not yet used */
static int device_is_open = 0;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nicu Pavel <npavel@ituner.com>");
MODULE_DESCRIPTION("implements process info for MediaBox (tm) embedded systems");
MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "turns pidwatcher module debug off/on (0/1)");
EXPORT_NO_SYMBOLS;


static struct get_info_struct *pidwatcher_store(void)
{
	struct task_struct *task;
	struct mm_struct *mm;
	struct pid_info_struct *ptr;
	struct get_info_struct *info;
	int pids = 0;
	unsigned long seconds_from_boot = jiffies / HZ;
	unsigned long seconds;
	
	if ((info = vmalloc(sizeof (*info))) == NULL)
	    return NULL;
	
	info->len = nr_threads * sizeof (struct pid_info_struct);
	
	DEBUG(printk (KERN_INFO "pidwatcher: allocated %d for processes\n",info->len));
	
	if ((info->data = vmalloc (info->len)) == NULL)
	    return NULL;
	memset (info->data, 0, info->len);
	    	    
	ptr = info->data;
	
	read_lock(&tasklist_lock);
	for_each_task(task) {
		pids++;
		task_lock(task);
		mm = task->mm;
		if (mm)
			atomic_inc(&mm->mm_users);
		task_unlock(task);

		if (mm) {
			int len = mm->env_end - mm->env_start;
			
			/*
			if (len > PAGE_SIZE)
				len = PAGE_SIZE;
			*/
			
			/*put the enviroment of the process*/
			ptr->environ_length = access_process_vm(task, mm->env_start, ptr->environ, len, 0);
			mmput(mm); 
		}
		
		/*put the other info  of the process*/
		ptr->pid = task->pid;
		/* cpu utilisation of the process */
		seconds = (seconds_from_boot - (task->start_time / HZ));
		if (seconds) ptr->cpu = ((task->times.tms_utime + task->times.tms_stime) * 1000 / HZ ) / seconds;
		else ptr->cpu = 0;
		
		ptr++;
	
	}
	read_unlock(&tasklist_lock);
	DEBUG(printk (KERN_INFO "pidwatcher: %d processes read.\n",pids));
	DEBUG(printk (KERN_INFO "pidwatcher: %d processes stored.\n",info->len / sizeof (struct pid_info_struct)));
	return info;

}

static void pidwatcher_release (struct get_info_struct *info)
{
    if (info != NULL) {
	DEBUG(printk (KERN_INFO "pidwatcher: %d processes released.\n",info->len / sizeof (struct pid_info_struct)));
	vfree(info->data);
	vfree(info);
    }
}

static int pidwatcher_device_open(struct inode *inode,struct file *file)
{
    DEBUG(printk (KERN_INFO "pidwatcher: device_open(%p,%p).\n",inode,file));
    DEBUG(printk (KERN_INFO "pidwatcher: device: %d.%d\n",
		    inode->i_rdev >> 8, inode->i_rdev & 0xFF));
    device_is_open++;
    MOD_INC_USE_COUNT;
    
    file->private_data = NULL;
    
    if ((file->private_data = pidwatcher_store()) == NULL) {
	MOD_DEC_USE_COUNT;
	return -ENOMEM;
    }
    return SUCCESS;
}

static int pidwatcher_device_release(struct inode *inode,struct file *file)
{
    DEBUG(printk(KERN_INFO "pidwatcher: device_release(%p,%p).\n",inode,file))
    
    pidwatcher_release ((struct get_info_struct *)(file->private_data));
    device_is_open--;
    MOD_DEC_USE_COUNT;
    
    return SUCCESS;
}


static ssize_t pidwatcher_device_read(struct file *file,char *buffer,size_t length,loff_t *offset)
{
    
    struct get_info_struct *info = (struct get_info_struct *)(file->private_data);

	if (*offset >= info->len)
		return 0;
	if (*offset + length > info->len)
		length = info->len - *offset;
	if (copy_to_user(buffer, (char *)info->data + *offset, length) < 0)
		return -EFAULT;
        *offset += length;
        return length;
}

static ssize_t pidwatcher_device_write (struct file *file,const char *buffer,size_t length,loff_t *offset)
{
    printk(KERN_INFO "pidwatcher: device is read-only.\n");
    return -EINVAL;
}

static struct file_operations pidwatcher_fops = {
    open:	pidwatcher_device_open,
    release:	pidwatcher_device_release,
    read:	pidwatcher_device_read,
    write:	pidwatcher_device_write,
};


static int __init pidwatcher_init_module(void)
{
    DEBUG(printk (KERN_INFO "pidwatcher version %s: begin loading.\n", PIDWATCHER_VERSION));
    major = register_chrdev(0,DEVICE_NAME,&pidwatcher_fops);
    if ( major < 0 ) {
	printk ("%s device failed with %d\n","pidwatcher: registering ",major);
	return major;
    }
    DEBUG(printk (KERN_INFO "pidwatcher: module registered with %d major number.\n",major));
    return SUCCESS;
}


static void __exit pidwatcher_cleanup_module(void)
{
    int ret;
    DEBUG(printk (KERN_INFO "pidwatcher: begin unload.\n"));    
    ret = unregister_chrdev(major,DEVICE_NAME);
    if ( ret < 0 ) printk ("pidwatcher: unregister device %d error: %d\n",major,ret);
    else DEBUG(printk (KERN_INFO "pidwatcher: succesfully unregister device %d.\n",major));    
}

module_init(pidwatcher_init_module);
module_exit(pidwatcher_cleanup_module);

