#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x8a2e525e, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x797e93d2, "class_destroy" },
	{ 0x71dc4c74, "device_destroy" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xcf8c0dfd, "device_create" },
	{ 0x3294966, "__class_create" },
	{ 0xec95baea, "__register_chrdev" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x72542c85, "__wake_up" },
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0x3a8ad4dc, "interruptible_sleep_on" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x27e1a049, "printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xa170bbdb, "outer_cache" },
	{ 0xff178f6, "__aeabi_idivmod" },
	{ 0x2196324, "__aeabi_idiv" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

