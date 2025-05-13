#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DRIVER_AUTHOR "Thuong"
#define DRIVER_DESC "Hello world kernel module"
#define DRIVER_VERS "1.0"

struct m_foo_dev {
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;

// Function Prototype
static int      __init hello_world_init(void);
static void     __exit hello_world_exit(void);
static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t   m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t   m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset);

static struct file_operations fops = 
{
    .owner      = THIS_MODULE,
    .read       = m_read,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release
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

// This function will be called when we READ the Device file
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call read() called ... !!!\n");
    return 0;
}

// This function will be called when we WRITE the Device file
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call write() called ... !!!\n");
    return size;
}

// Constructor
static int __init hello_world_init(void)
{
    // 01. Dynamic allocating device number (cat /proc/devices)
    if (alloc_chrdev_region(&mdev.dev_num,0, 1, "m-cdev") <0){
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


    // 02. Creating struct class
    if ((mdev.m_class = class_create(THIS_MODULE,"m_class")) == NULL){
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }
    
    // 03. Creating device
    if ((device_create(mdev.m_class, NULL, mdev.dev_num,NULL, "m_device")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }

    pr_info("Hello world kernel module\n");
    return 0;

rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

// Destructor
static void __exit hello_world_exit(void)
{
    // Unallocating device number
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    unregister_chrdev_region(mdev.dev_num, 1);
    pr_info("Goodbye\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DRIVER_AUTHOR");
MODULE_DESCRIPTION("DRIVER_DESC");
MODULE_VERSION("DRIVER_VERSION");

