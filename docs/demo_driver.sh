#!/bin/bash
# Quick demonstration of the driver without the application.
# Run from the project root:  bash scripts/demo_driver.sh
cd "$(dirname "$0")/.." || exit 1

echo "== 1. Build the kernel module =="
make -C driver || exit 1

echo; echo "== 2. Load it and check the device node =="
sudo rmmod ccms_log 2>/dev/null
sudo insmod driver/ccms_log.ko || exit 1
sudo chmod 666 /dev/ccms_log
ls -l /dev/ccms_log

echo; echo "== 3. Kernel messages =="
sudo dmesg | tail -n 2

echo; echo "== 4. Write two events to the device =="
echo "[demo] STUDENT_ADDED id=1" > /dev/ccms_log
echo "[demo] RESULT_ADDED student=1 grade=A" > /dev/ccms_log

echo; echo "== 5. Read the log back =="
cat /dev/ccms_log

echo; echo "== 6. Unload the driver =="
sudo rmmod ccms_log
sudo dmesg | tail -n 2
