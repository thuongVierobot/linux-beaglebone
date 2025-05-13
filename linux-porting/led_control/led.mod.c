#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x6fbd61d8, "module_layout" },
	{ 0xedc03953, "iounmap" },
	{ 0x4d43d4d2, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x71a5f1ed, "class_destroy" },
	{ 0xd26b5ad8, "device_destroy" },
	{ 0xe97c4103, "ioremap" },
	{ 0xb9c8e072, "device_create" },
	{ 0x6e281da3, "__class_create" },
	{ 0x9da20cef, "cdev_add" },
	{ 0x9c4b2604, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "45DBB56F0B1773EF6A2CA50");
