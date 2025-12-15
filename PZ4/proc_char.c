#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "myprocdev"
#define PROC_NAME "myproc_status"
#define BUFFER_SIZE 256

// IOCTL команды
#define IOCTL_MAGIC 'k'
#define IOCTL_RESET_BUFFER      _IO(IOCTL_MAGIC, 0)
#define IOCTL_GET_BUFFER_STATUS _IOR(IOCTL_MAGIC, 1, int)

static int major_num;
static char *buffer;
static size_t buffer_len = 0;
static struct class *my_class = NULL;
static struct device *my_device = NULL;
static struct proc_dir_entry *proc_entry = NULL;

// === Работа с /proc ===
static int proc_show(struct seq_file *m, void *v) {
    if (buffer_len > 0) {
        seq_printf(m, "Status: buffer full (%zu bytes)\nData: %s\n", buffer_len, buffer);
    } else {
        seq_printf(m, "Status: buffer empty\n");
    }
    return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
    return single_open(file, proc_show, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

// === Операции устройства ===
static ssize_t device_read(struct file *file, char __user *user_buf,
                           size_t count, loff_t *off) {
    size_t len = min(count, buffer_len);
    if (copy_to_user(user_buf, buffer, len))
        return -EFAULT;
    return len;
}

static ssize_t device_write(struct file *file, const char __user *user_buf,
                            size_t count, loff_t *off) {
    size_t len = min(count, (size_t)(BUFFER_SIZE - 1));
    if (copy_from_user(buffer, user_buf, len))
        return -EFAULT;
    buffer[len] = '\0';
    buffer_len = len;
    return len;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int status;
    switch (cmd) {
        case IOCTL_RESET_BUFFER:
            buffer[0] = '\0';
            buffer_len = 0;
            printk(KERN_INFO "myprocdev: buffer reset\n");
            return 0;

        case IOCTL_GET_BUFFER_STATUS:
            status = (buffer_len > 0) ? 1 : 0;
            if (copy_to_user((int __user *)arg, &status, sizeof(int)))
                return -EFAULT;
            return 0;

        default:
            return -ENOTTY;
    }
}

static const struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
    .owner = THIS_MODULE,
};

// === Инициализация ===
static int __init proc_char_init(void) {
    // Регистрация символьного устройства
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        printk(KERN_ERR "myprocdev: Failed to register char device\n");
        return major_num;
    }

    // Создание класса (новый API: только имя!)
    my_class = class_create("myproc_class");
    if (IS_ERR(my_class)) {
        unregister_chrdev(major_num, DEVICE_NAME);
        return PTR_ERR(my_class);
    }

    // Создание устройства
    my_device = device_create(my_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major_num, DEVICE_NAME);
        return PTR_ERR(my_device);
    }

    // Выделение буфера
    buffer = kzalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!buffer) {
        device_destroy(my_class, MKDEV(major_num, 0));
        class_destroy(my_class);
        unregister_chrdev(major_num, DEVICE_NAME);
        return -ENOMEM;
    }

    // Создание /proc файла
    proc_entry = proc_create(PROC_NAME, 0444, NULL, &proc_fops);
    if (!proc_entry) {
        kfree(buffer);
        device_destroy(my_class, MKDEV(major_num, 0));
        class_destroy(my_class);
        unregister_chrdev(major_num, DEVICE_NAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "myprocdev: module loaded, device /dev/%s created, /proc/%s available\n", DEVICE_NAME, PROC_NAME);
    return 0;
}

// === Выгрузка ===
static void __exit proc_char_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    kfree(buffer);
    device_destroy(my_class, MKDEV(major_num, 0));
    class_destroy(my_class);
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "myprocdev: module unloaded\n");
}

module_init(proc_char_init);
module_exit(proc_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Char driver with /proc status and ioctl");
MODULE_VERSION("1.0");