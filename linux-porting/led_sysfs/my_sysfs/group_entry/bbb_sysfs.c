#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/gpio.h>

#define DRIVER_AUTHOR   "Thuong"
#define DRIVER_DESC     "GPIO sysfs driver"

// Static variable
static unsigned int GPIO_LED = 31;
static int32_t _value = 0;
static char _direct[8] = {};



struct m_foo_dev{
    struct kobject *kobj_ref;
} mdev;

// Function prototypes
static int      __init hello_world_init(void);
static void     __exit hello_world_exit(void);

// Sysfs function
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);
struct kobj_attribute value = __ATTR(value, 0660, value_show, value_store);

// This function will be called when we read the sysfs file
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%s", _direct);
}

// This function will be called when we write the sysfs file
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    char *Direction;
    sscanf(buf, "%s", &Direction);
    switch(count - 1){
        case 2: //off
            if(!strncmp(Direction,"in",2)){
                gpio_direction_input(GPIO_LED);
                break;
            }
        case 3: //on
            if(!strncmp(Direction,"out",3)){
                gpio_direction_input(GPIO_LED);
                break;
        }
        default:
            return count;
    }
    sscanf(buf, "%s", &_direct);
    return count;
}

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d", _value);
}

// This function will be called when we write the sysfs file
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32_t numb =0;
    sscanf(buf, "%d", &numb);
    
    switch(numb){
        case 0: //off
            gpio_set_value(GPIO_LED, 0);
            break;
        case 1: //on
            gpio_set_value(GPIO_LED, 1);
            break;
        default:
            return count;
    }
    sscanf(buf, "%d", &_value);
    return count;
}

//Init group attribute
static struct attribute * attrs[] = {
    &direction.attr,
    &value.attr,
    NULL,
};

static struct attribute_group attr_group = {
   .attrs = attrs, 
};

// Constructor
static int __init hello_world_init(void)
{
    // 01. It will create a directory under "/sys" [firmware_kobj, class_kobj]
    mdev.kobj_ref = kobject_create_and_add("bbb_gpios", NULL);

    // 02. Creating sys entry under "/sys/bbb_gpios/"
    if (sysfs_create_group(mdev.kobj_ref, &attr_group)){
        pr_info("Cannot create sys file.....\n");
        goto rm_kobj;
    }
    // 03. Request GPIO
    gpio_request(GPIO_LED, "led");
    gpio_direction_output(GPIO_LED, 0);

    pr_info("Hello! Initialize successfully!");
    return 0;

rm_kobj:
    kobject_put(mdev.kobj_ref);
    return -1;
}

static void __exit hello_world_exit(void)
{
    gpio_free(GPIO_LED);
    sysfs_remove_group(mdev.kobj_ref, &attr_group);
    kobject_put(mdev.kobj_ref);
    pr_info("Goodbye my fen!!!\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");