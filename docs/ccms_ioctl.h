/*
 * ccms_ioctl.h - definitions shared by the CCMS kernel driver and the
 * College Management System application (user space).
 */
#ifndef CCMS_IOCTL_H
#define CCMS_IOCTL_H

#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif

#define CCMS_LOG_SIZE      8192    /* capacity of the event log in bytes */

#define CCMS_IOC_MAGIC     'c'
#define CCMS_IOC_GET_COUNT _IOR(CCMS_IOC_MAGIC, 1, int)  /* number of events */
#define CCMS_IOC_GET_USED  _IOR(CCMS_IOC_MAGIC, 2, int)  /* bytes used       */
#define CCMS_IOC_CLEAR     _IO(CCMS_IOC_MAGIC, 3)        /* erase the log    */

#endif /* CCMS_IOCTL_H */
