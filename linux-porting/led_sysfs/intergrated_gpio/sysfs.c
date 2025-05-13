#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define DRIVER_AUTHOR "Thuong"
#define DRIVER_DESC   "GPIO subsystem"

static unsigned int LED = 30;

//Constructor
static int __init gpio_init(void){
    //int retval;
    gpio_request(LED, "led");
    gpio_direction_output(LED, 0); // Set output and initial value
    gpio_set_value(LED, 1); // Turn on LED
    pr_info("Hello! Initialize successfully\n");
    return 0;
}

static void __exit gpio_exit(void){
    gpio_set_value(LED, 0); // Turn off LED
    gpio_free(LED);  // Release GPIO pin
    pr_info("Exit mode... Goodbye my fen :_< \n");
}

module_init(gpio_init); 
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");