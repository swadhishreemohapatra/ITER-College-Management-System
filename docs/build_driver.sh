#!/bin/bash
# Build the kernel module (Linux only).
# Needs:  sudo apt install build-essential linux-headers-$(uname -r)
# Run from the project root:  bash scripts/build_driver.sh
cd "$(dirname "$0")/.." || exit 1
make -C driver && echo "Built: driver/ccms_log.ko"
