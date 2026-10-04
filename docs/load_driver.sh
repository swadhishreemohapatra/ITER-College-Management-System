#!/bin/bash
# Load the driver and make /dev/ccms_log usable by a normal user.
# Run from the project root:  bash scripts/load_driver.sh
cd "$(dirname "$0")/.." || exit 1
sudo insmod driver/ccms_log.ko || exit 1
sudo chmod 666 /dev/ccms_log
ls -l /dev/ccms_log
sudo dmesg | tail -n 2
