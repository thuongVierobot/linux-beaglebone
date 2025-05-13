#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include<linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include<linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */
#include <linux/io.h>
#include "blink.h"

#define DRIVER_AUTHOR "Thuong"
#define DRIVER_DESC "Hello world kernel module"

#define NPAGES 1

#define LED_ON _IOW('a','1', int32_t*)
#define LED_OFF _IOW('a','0', int32_t*)

uint32_t   __iomem  *base_addr;
uint32_t set_val, clear_val;
//static int32_t value = 0;

struct m_foo_dev {
    int size;
    //char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;

// Function Prototype
static int      __init hello_world_init(void);
static void     __exit hello_world_exit(void);
static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static long     m_ioctl(struct file *file, unsigned int cmd, unsigned long arg);


static struct file_operations fops = 
{
    .owner      = THIS_MODULE,
    .open       = m_open,
    .release    = m_release,
    .unlocked_ioctl = m_ioctl,
};

// This function will be called when we OPEN the Device file
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called ...!!!\n");
    return 0;
}

// This function will be called when we CLOSE the Device file
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called ...!!!\n");
    return 0;
}

static long m_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd){
        case LED_ON:
            pr_info("LED ON");
            *(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= LED;
            set_val = *(base_addr + GPIO_SETDATAOUT_OFFSET / 4);
            printk("SETDATAOUT after: 0x%x\n", set_val);
            break;

        case LED_OFF:
            pr_info("LED OFF");
            *(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED;
            clear_val = *(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4);
            printk("CLEARDATAOUT after: 0x%x\n", clear_val);       
            break;

        default:
            pr_info("EXIT.....\n");
            break;
    }
    return 0;
}


// Constructor
static int __init hello_world_init(void)
{
    // 01. Dynamic allocating device number (cat /proc/devices)
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "m-cdev") <0){
        pr_err("Failed to alloc chrdev region\n");
        return -1;
    }

    // Static allocating device number (cat /proc/devices)
    // register_chrdev_region(&mdev.devnum, 1, "m-cdev");

    pr_info("Major = %d Miror = %d\n",MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    // 02.1 Creating cdev structure
    cdev_init(&mdev.m_cdev, &fops);

    // 02.2 Adding character device to the system
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0){
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }


    // 03. Creating struct class
    if ((mdev.m_class = class_create(THIS_MODULE,"m_class")) == NULL){
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }
    
    // 04. Creating device
    if ((device_create(mdev.m_class, NULL, mdev.dev_num,NULL, "m_device")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }
    /* 05. Creating Physical Memory
    if ((mdev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL)) ==0){
        pr_err("Cannot allocate memory in kernel\n");
        goto rm_device;
    }
         */

    // 05. Map GPIO
    base_addr = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
    if (!base_addr){
        pr_err("Map GPIO error!!!");
        goto rm_device;
        return -ENOMEM;
    }

    // Set led as output
    *(base_addr + GPIO_OE_OFFSET / 4) &= ~LED;
    pr_info("GPIO_OE = %x\n", *(base_addr + GPIO_OE_OFFSET / 4));
    *(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= LED;
    pr_info("Initialize successfully\n");
    return 0;

rm_device:
    device_destroy(mdev.m_class, mdev.dev_num);
    pr_info("Destroyed device!!!");
rm_class:
    class_destroy(mdev.m_class);
    pr_info("Class destroy!!!");
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    pr_info("Deleted device number!!!");
    return -1;
}

// Destructor
static void __exit hello_world_exit(void)
{
    // Unallocating device number
    //kfree(mdev.kmalloc_ptr);
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    cdev_del(&mdev.m_cdev);
    unregister_chrdev_region(mdev.dev_num, 1);
    *(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED;
    iounmap(base_addr);
    pr_info("Goodbye\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");

