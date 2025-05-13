#include <linux/module.h>
#include <linux/io.h>
#include "blink.h"

#define DRIVER_AUTHOR "Thuong"
#define DRIVER_DESC   "Led Blink"

uint32_t   __iomem  *base_addr;

//Constructor
static int __init led_init(void){
    base_addr = ioremap(GPIO_0_ADDR_BASE, GPIO_0_ADDR_SIZE);
    if (!base_addr)
        return -ENOMEM;
    *(base_addr + GPIO_OE_OFFSET / 4) &= ~LED;
    *(base_addr + GPIO_SETDATAOUT_OFFSET / 4) |= LED;

    pr_info("Hello initizalize successfully!\n");
    return 0;
}

// Destructor

static void __exit led_exit(void){
    *(base_addr + GPIO_CLEARDATAOUT_OFFSET / 4) |= LED;
    iounmap(base_addr);
    pr_info("Goodbye my fen!!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");