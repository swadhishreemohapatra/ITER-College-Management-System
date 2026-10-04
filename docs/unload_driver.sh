#!/bin/bash
# Unload the driver.
sudo rmmod ccms_log && sudo dmesg | tail -n 2
