#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "mydevice"
#define BUFFER_SIZE 256

static int major_num;
static char *device_buffer;
static int buffer_ptr;

// Вызывается при open()
static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydriver: open\n");
    return 0;
}

// Вызывается при read()
static ssize_t device_read(struct file *file, char __user *buf,
                           size_t count, loff_t *pos) {
    int bytes_read = 0;
    if (*pos >= buffer_ptr) {
        return 0; // EOF
    }
    if (count > (buffer_ptr - *pos)) {
        count = buffer_ptr - *pos;
    }
    if (copy_to_user(buf, device_buffer + *pos, count)) {
        return -EFAULT;
    }
    *pos += count;
    bytes_read = count;
    printk(KERN_INFO "mydriver: read %d bytes\n", bytes_read);
    return bytes_read;
}

// Вызывается при write()
static ssize_t device_write(struct file *file, const char __user *buf,
                            size_t count, loff_t *pos) {
    int bytes_written = 0;
    if (count > (BUFFER_SIZE - buffer_ptr)) {
        count = BUFFER_SIZE - buffer_ptr;
    }
    if (copy_from_user(device_buffer + buffer_ptr, buf, count)) {
        return -EFAULT;
    }
    buffer_ptr += count;
    bytes_written = count;
    printk(KERN_INFO "mydriver: wrote %d bytes\n", bytes_written);
    return bytes_written;
}

// Вызывается при close()
static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mydriver: close\n");
    return 0;
}

// Операции устройства
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

// Инициализация модуля
static int __init mydriver_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        printk(KERN_ALERT "mydriver: failed to register char device\n");
        return major_num;
    }
    printk(KERN_INFO "mydriver: registered with major number %d\n", major_num);

    device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!device_buffer) {
        printk(KERN_ALERT "mydriver: failed to allocate buffer\n");
        unregister_chrdev(major_num, DEVICE_NAME);
        return -ENOMEM;
    }
    buffer_ptr = 0;
    printk(KERN_INFO "mydriver: buffer allocated\n");
    return 0;
}

// Выгрузка модуля
static void __exit mydriver_exit(void) {
    kfree(device_buffer);
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "mydriver: module removed\n");
}

module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple char device driver");