/*
 * ccms_log.c - Linux character device driver (kernel module)
 *
 * Creates /dev/ccms_log, an audit-log device for the College Management
 * System. The application writes one line per event (student added,
 * result saved, ...). Reading the device returns the whole log, and
 * ioctl() reports statistics or clears the log.
 *
 * Driver concepts used: module init/exit, character device registration
 * (alloc_chrdev_region, cdev), automatic device node (class/device),
 * file_operations (open, release, read, write, unlocked_ioctl),
 * copy_from_user / put_user, mutex locking, kernel logging (dmesg).
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/version.h>

#include "ccms_ioctl.h"

#define DEVICE_NAME "ccms_log"
#define MAX_MSG     256            /* longest single event accepted */

static dev_t dev_num;
static struct cdev ccms_cdev;
static struct class *ccms_class;
static DEFINE_MUTEX(ccms_lock);

static char log_buf[CCMS_LOG_SIZE];
static size_t log_len;             /* bytes stored in log_buf */
static int event_count;            /* number of events written */

static int ccms_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int ccms_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* Append one event to the log. */
static ssize_t ccms_write(struct file *filp, const char __user *ubuf,
                          size_t count, loff_t *ppos)
{
    if (count == 0)
        return 0;
    if (count > MAX_MSG)
        count = MAX_MSG;

    mutex_lock(&ccms_lock);

    if (log_len + count > CCMS_LOG_SIZE) {
        mutex_unlock(&ccms_lock);
        return -ENOSPC;            /* log full: clear it with ioctl */
    }
    if (copy_from_user(log_buf + log_len, ubuf, count)) {
        mutex_unlock(&ccms_lock);
        return -EFAULT;
    }
    log_len += count;
    event_count++;

    mutex_unlock(&ccms_lock);
    return count;
}

/* Return the stored log to the reader. */
static ssize_t ccms_read(struct file *filp, char __user *ubuf,
                         size_t count, loff_t *ppos)
{
    ssize_t ret;

    mutex_lock(&ccms_lock);
    ret = simple_read_from_buffer(ubuf, count, ppos, log_buf, log_len);
    mutex_unlock(&ccms_lock);
    return ret;
}

static long ccms_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int value;

    if (_IOC_TYPE(cmd) != CCMS_IOC_MAGIC)
        return -ENOTTY;

    switch (cmd) {
    case CCMS_IOC_GET_COUNT:
        mutex_lock(&ccms_lock);
        value = event_count;
        mutex_unlock(&ccms_lock);
        if (put_user(value, (int __user *)arg))
            return -EFAULT;
        return 0;

    case CCMS_IOC_GET_USED:
        mutex_lock(&ccms_lock);
        value = (int)log_len;
        mutex_unlock(&ccms_lock);
        if (put_user(value, (int __user *)arg))
            return -EFAULT;
        return 0;

    case CCMS_IOC_CLEAR:
        mutex_lock(&ccms_lock);
        log_len = 0;
        event_count = 0;
        mutex_unlock(&ccms_lock);
        pr_info("ccms_log: event log cleared\n");
        return 0;

    default:
        return -ENOTTY;
    }
}

static const struct file_operations ccms_fops = {
    .owner          = THIS_MODULE,
    .open           = ccms_open,
    .release        = ccms_release,
    .read           = ccms_read,
    .write          = ccms_write,
    .unlocked_ioctl = ccms_ioctl,
};

static int __init ccms_init(void)
{
    int ret;
    struct device *dev;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&ccms_cdev, &ccms_fops);
    ccms_cdev.owner = THIS_MODULE;
    ret = cdev_add(&ccms_cdev, dev_num, 1);
    if (ret)
        goto err_region;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    ccms_class = class_create(DEVICE_NAME);
#else
    ccms_class = class_create(THIS_MODULE, DEVICE_NAME);
#endif
    if (IS_ERR(ccms_class)) {
        ret = PTR_ERR(ccms_class);
        goto err_cdev;
    }

    dev = device_create(ccms_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        ret = PTR_ERR(dev);
        goto err_class;
    }

    pr_info("ccms_log: loaded, device /dev/%s (major %d)\n",
            DEVICE_NAME, MAJOR(dev_num));
    return 0;

err_class:
    class_destroy(ccms_class);
err_cdev:
    cdev_del(&ccms_cdev);
err_region:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

static void __exit ccms_exit(void)
{
    device_destroy(ccms_class, dev_num);
    class_destroy(ccms_class);
    cdev_del(&ccms_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("ccms_log: unloaded\n");
}

module_init(ccms_init);
module_exit(ccms_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("College Management System project");
MODULE_DESCRIPTION("Audit-log character device for the College Management System");
