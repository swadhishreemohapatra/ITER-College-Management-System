# College Management System

**Institute Of Technical Education And Research, Bhubaneswar**

A menu-driven console application written in **C** that manages the day-to-day records of a college:
students, faculty, courses, attendance, marks/results and fees. Data is stored in binary `.dat` files.
On Linux the project also includes a **kernel module (character device driver)**, `/dev/ccms_log`, that keeps an
audit log of everything the application does.

## Features

| # | Module | Operations |
|---|--------|------------|
| 1 | Admin Login | Username/password check, 3 attempts |
| 2 | Student Management | Add, View, Search, Update, Delete |
| 3 | Faculty Management | Add, View, Search |
| 4 | Course Management | Add, View |
| 5 | Attendance Management | Add, View report, automatic percentage, warning below 75% |
| 6 | Result / Marks Management | Add, View report, automatic total and grade (A+ to F) |
| 7 | Fee Management | Add, View report, automatic due amount, PAID / DUE status |
| 8 | College Dashboard | Total students, faculty and courses |
| 9 | Driver Event Log (Linux) | View the log kept by the kernel driver, driver statistics (ioctl), clear the log (ioctl) |

Data validation: duplicate Student / Faculty IDs and Course codes are rejected; attendance, results and fees
are accepted only for students and courses that already exist; marks, attendance and fee values are range-checked.

## Linux device driver

`driver/ccms_log.c` is a Linux **character device driver** (loadable kernel module). It creates `/dev/ccms_log`:

* `write()` appends one event line to an 8 KB log in kernel memory (the application sends events such as
  `STUDENT_ADDED id=1`).
* `read()` returns the stored log (`cat /dev/ccms_log`).
* `ioctl()` returns the event count (`CCMS_IOC_GET_COUNT`), the bytes used (`CCMS_IOC_GET_USED`) and clears the
  log (`CCMS_IOC_CLEAR`).
* A mutex protects the buffer; `dmesg` shows load, unload and clear messages.

The application uses the driver only on Linux. On Windows the same source builds and runs normally and menu
option 8 reports that the driver is not available. Details: [`docs/Driver_Guide.md`](docs/Driver_Guide.md).

## Project structure

```
CollegeManagementSystem/
├── src/
│   └── main.c                # application (C)
├── driver/
│   ├── ccms_log.c            # Linux kernel module (character device driver)
│   ├── ccms_ioctl.h          # constants shared by driver and application
│   └── Makefile              # kernel module build file
├── scripts/                  # build, load, unload and demo scripts (Linux)
├── docs/
│   ├── Stage1_Project_Introduction.md
│   ├── Stage2_Requirements_and_Plan.md
│   ├── Stage3_System_Design.md
│   ├── Stage4_Prototype_and_Progress.md
│   ├── Stage5_Testing.md
│   ├── Stage6_Final_Report.md
│   ├── Driver_Guide.md
│   └── screenshots/          # evidence screenshots
├── README.md
└── .gitignore
```

## Build and run

### Application on Windows 11 (MSYS2 UCRT64)

1. Install MSYS2 from https://www.msys2.org and open the **MSYS2 UCRT64** terminal.
2. Install the compiler (one time): `pacman -S mingw-w64-ucrt-x86_64-gcc`
3. `cd /c/Users/<you>/Desktop/CollegeManagementSystem`
4. `gcc src/main.c -o college_management`
5. `./college_management.exe`

### Application and driver on Linux (Ubuntu)

```
sudo apt install build-essential linux-headers-$(uname -r)
bash scripts/build_app.sh          # builds ./college_management
bash scripts/build_driver.sh       # builds driver/ccms_log.ko
bash scripts/load_driver.sh        # insmod + permissions on /dev/ccms_log
./college_management               # menu 8 shows the driver log
bash scripts/unload_driver.sh      # rmmod
```

Quick driver-only demo: `bash scripts/demo_driver.sh`

**Default login:** username `admin`, password `admin123`

The program creates its `.dat` data files in the folder you run it from.

## Technical notes

* Language: C (standard library, plus POSIX `open/read/write/ioctl` on Linux).
* Kernel module: GPL-licensed, written for Linux 5.x/6.x (the `class_create` API change in 6.4 is handled).
* Storage: one binary file per module, written with `fwrite` and read with `fread`.
* Application tested on Windows 11 with GCC under MSYS2 UCRT64.

## Documentation

The project was developed in six stages; the documents are in the [`docs`](docs) folder.

## Known limitations

* Only the Student module supports Update and Delete.
* The administrator credentials are fixed in the source code.
* Records are stored in plain binary files without encryption.
* The driver log lives in kernel memory and is lost when the module is unloaded or the machine restarts.
* Console only: no graphical interface.
