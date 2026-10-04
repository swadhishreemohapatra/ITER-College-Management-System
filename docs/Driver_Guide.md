# Driver Guide - `ccms_log`

The driver is a Linux kernel module. It runs only on Linux; it cannot be loaded on Windows.

## 1. What you need
* Linux (Ubuntu 22.04 or later, on a real machine, a virtual machine or a college lab PC)
* `sudo apt install build-essential linux-headers-$(uname -r)`
* Secure Boot **off**, or the module signed (see Troubleshooting)

## 2. Build
```
cd driver
make
```
Result: `ccms_log.ko`. Clean with `make clean`.

## 3. Load and check
```
sudo insmod ccms_log.ko
sudo chmod 666 /dev/ccms_log
ls -l /dev/ccms_log
sudo dmesg | tail
```
`dmesg` should show `ccms_log: loaded, device /dev/ccms_log (major NNN)`.
Check that the module is loaded with `lsmod | grep ccms`.

## 4. Use it without the application
```
echo "first event" > /dev/ccms_log
echo "second event" > /dev/ccms_log
cat /dev/ccms_log
```
Note: `>` opens the device for each command; every `echo` is one event. Use `>>` if you prefer.

## 5. Use it with the application
```
gcc src/main.c -o college_management
./college_management
```
Log in, add a student, then choose **8. Driver Event Log**:
1. View Event Log - the events written by the application
2. Show Driver Statistics - event count and bytes used (ioctl)
3. Clear Event Log (ioctl)

## 6. Unload
```
sudo rmmod ccms_log
sudo dmesg | tail
```
The device node `/dev/ccms_log` disappears.

## 7. Where each concept is in the code
| Concept | File and function |
|---------|-------------------|
| Module entry and exit | `ccms_log.c`: `ccms_init`, `ccms_exit` |
| Device number and cdev | `ccms_init`: `alloc_chrdev_region`, `cdev_init`, `cdev_add` |
| Device node | `ccms_init`: `class_create`, `device_create` |
| write | `ccms_write`: `copy_from_user`, bounds checks, mutex |
| read | `ccms_read`: `simple_read_from_buffer` |
| ioctl | `ccms_ioctl`; command numbers in `ccms_ioctl.h` |
| Application side | `main.c`: `logEvent`, `viewDriverLog`, `showDriverStats`, `clearDriverLog` |

## 8. Troubleshooting

| Problem | Likely cause and fix |
|---------|----------------------|
| `make`: `/lib/modules/.../build: No such file or directory` | Install the headers: `sudo apt install linux-headers-$(uname -r)` |
| `insmod: Operation not permitted` or `Key was rejected by service` | Secure Boot blocks unsigned modules. Turn Secure Boot off in the BIOS, or sign the module |
| `insmod: File exists` | Already loaded: `sudo rmmod ccms_log` then load again |
| Application says the device could not be opened | Driver not loaded, or permissions: `sudo chmod 666 /dev/ccms_log` |
| `echo > /dev/ccms_log` gives `Permission denied` | Run `sudo chmod 666 /dev/ccms_log` after every load |
| Compile error about `class_create` | Kernel API differs; the code handles 5.x and 6.4+. Send the exact error message |
| WSL | The default WSL kernel has no module headers; use a normal Ubuntu install, live USB or virtual machine instead |
