# Execution Guide

## Operating System

The project is intended for Linux/Ubuntu.

## Install GCC

```bash
sudo apt update
sudo apt install gcc
```

## Compile

From the project directory:

```bash
gcc src/main.c -o college_management
```

## Run

```bash
./college_management
```

## Login

```text
Username: admin
Password: admin123
```

## Modules

1. Student Management
2. Faculty Management
3. Course Management
4. Attendance Management
5. Result Management
6. Fee Management
7. Dashboard
8. Exit

## Data Files

The program generates binary files such as:

- students.dat
- faculty.dat
- courses.dat
- attendance.dat
- results.dat
- fees.dat

These files are excluded from GitHub by `.gitignore`.

## Testing Note

The current development version was functionally tested on Windows using MSYS2 UCRT64. The source contains Linux-specific terminal handling and is intended for Linux/GCC execution. Ubuntu execution should be verified on an actual Linux system before describing it as Linux-tested.
