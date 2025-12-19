#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>

#define DEVICE_NAME "timerdev"
#define HISTOGRAM_BINS 20
#define BIN_SIZE_US 50  // 50 микросекунд на бин

static int major_num;
static int32_t last_value = 0;
static s64 write_timestamp_ns = 0;  // время последней записи в наносекундах
static unsigned long histogram[HISTOGRAM_BINS] = {0};
static DEFINE_SPINLOCK(histogram_lock);

// IOCTL команды
#define TIMER_IOC_MAGIC 't'
#define TIMER_IOC_GET_HISTOGRAM _IO(TIMER_IOC_MAGIC, 0)

// Вызывается при write()
static ssize_t device_write(struct file *file, const char __user *buf,
                            size_t count, loff_t *pos)
{
    int32_t value;
    if (count != sizeof(int32_t))
        return -EINVAL;

    if (copy_from_user(&value, buf, sizeof(value)))
        return -EFAULT;

    last_value = value;
    write_timestamp_ns = ktime_get_ns();  // точное время записи
    return sizeof(value);
}

// Вызывается при read()
static ssize_t device_read(struct file *file, char __user *buf,
                           size_t count, loff_t *pos)
{
    int32_t value;
    s64 read_time_ns, delta_us;

    if (count != sizeof(int32_t))
        return -EINVAL;

    if (write_timestamp_ns == 0)
        return -EAGAIN; // ещё не было записи

    value = last_value;
    read_time_ns = ktime_get_ns();
    delta_us = (read_time_ns - write_timestamp_ns) / 1000;  // в микросекунды

    // Обновляем гистограмму
    if (delta_us >= 0) {
        int bin = delta_us / BIN_SIZE_US;
        if (bin >= HISTOGRAM_BINS)
            bin = HISTOGRAM_BINS - 1;

        unsigned long flags;
        spin_lock_irqsave(&histogram_lock, flags);
        histogram[bin]++;
        spin_unlock_irqrestore(&histogram_lock, flags);
    }

    if (copy_to_user(buf, &value, sizeof(value)))
        return -EFAULT;

    return sizeof(value);
}

// Вызывается при ioctl()
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int i;
    if (_IOC_TYPE(cmd) != TIMER_IOC_MAGIC)
        return -ENOTTY;

    switch (cmd) {
    case TIMER_IOC_GET_HISTOGRAM:
        printk(KERN_INFO "timer_driver: Histogram (bin = %d us):\n", BIN_SIZE_US);
        for (i = 0; i < HISTOGRAM_BINS; i++) {
            printk(KERN_INFO "  bin[%02d] (%4d-%4d us): %lu\n",
                   i,
                   i * BIN_SIZE_US,
                   (i + 1) * BIN_SIZE_US - 1,
                   histogram[i]);
        }
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

// Инициализация
static int __init timer_driver_init(void)
{
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        printk(KERN_ALERT "timer_driver: failed to register char device\n");
        return major_num;
    }
    printk(KERN_INFO "timer_driver: registered with major %d\n", major_num);
    return 0;
}

// Выгрузка
static void __exit timer_driver_exit(void)
{
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "timer_driver: unregistered\n");
}

module_init(timer_driver_init);
module_exit(timer_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Timer driver with histogram");
MODULE_VERSION("1.0");