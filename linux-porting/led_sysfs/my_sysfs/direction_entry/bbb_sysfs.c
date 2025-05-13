#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DRIVER_AUTHOR   "Thuong"
#define DRIVER_DESC     "GPIO sysfs driver"

struct m_foo_dev{
    struct kobject *kobj_ref;
} mdev;

// Function prototypes
static int      __init hello_world_init(void);
static void     __exit hello_world_exit(void);

// Sysfs function
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

struct kobj_attribute m_attr = __ATTR(direction, 0660, sysfs_show, sysfs_store);

// This function will be called when we read the sysfs file
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    pr_info("Sysfs read.....\n");
    return 0;
}

// This function will be called when we write the sysfs file
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    pr_info("Sysfs write.....\n");
    return count;
}

// Constructor
static int __init hello_world_init(void)
{
    // 01. It will create a directory under "/sys" [firmware_kobj, class_kobj]
    mdev.kobj_ref = kobject_create_and_add("bbb_gpios", NULL);

    // 02. Creating sys entry under "/sys/bbb_gpios/"
    if (sysfs_create_file(mdev.kobj_ref, &m_attr.attr)){
        pr_info("Cannot create sys file.....\n");
        goto rm_kobj;
    }

    pr_info("Hello! Initialize successfully!");
    return 0;

rm_kobj:
    kobject_put(mdev.kobj_ref);
    return -1;
}

static void __exit hello_world_exit(void)
{
    sysfs_remove_file(mdev.kobj_ref, &m_attr.attr);
    kobject_put(mdev.kobj_ref);
    pr_info("Goodbye my fen!!!\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");