#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("A simple Hello World kernel module");
MODULE_VERSION("0.1");

// Функция, вызываемая при загрузке модуля
static int __init hello_init(void) {
    printk(KERN_INFO "Hello, world! Module loaded.\n");
    return 0;
}

// Функция, вызываемая при выгрузке модуля
static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye! Module unloaded.\n");
}

// Регистрируем функции инициализации и выхода
module_init(hello_init);
module_exit(hello_exit);