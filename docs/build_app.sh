#!/bin/bash
# Build the College Management System application (Linux or MSYS2).
# Run from the project root:  bash scripts/build_app.sh
cd "$(dirname "$0")/.." || exit 1
gcc -Wall -Wextra src/main.c -o college_management && echo "Built: ./college_management"
