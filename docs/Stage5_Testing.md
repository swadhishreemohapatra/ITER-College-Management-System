# Stage 5 - Testing, Integration and Improvement

## 1. Test environments

| Run | Environment | Method |
|-----|-------------|--------|
| A | Windows 11, MSYS2 UCRT64, GCC | Manual run through the menus; screenshots kept as evidence (2 Oct 2026) |
| B | Ubuntu 24.04, GCC 13.3 | Compile with `-Wall -Wextra` (no warnings) and scripted input run (3 Oct 2026) |
| C | Linux with kernel headers | Build, load and test the kernel module `ccms_log` (section 5.2). **To be completed and recorded on a Linux machine.** |

## 2. Unit tests (single functions)

| ID | Function | Input | Expected | Result |
|----|----------|-------|----------|--------|
| U-01 | calculateGrade | total 90 | A+ | Pass |
| U-02 | calculateGrade | total 89 | A | Pass |
| U-03 | calculateGrade | total 80 / 79 | A / B+ | Pass |
| U-04 | calculateGrade | total 70 / 69 | B+ / B | Pass |
| U-05 | calculateGrade | total 60 / 59 | B / C | Pass |
| U-06 | calculateGrade | total 50 / 49 | C / D | Pass |
| U-07 | calculateGrade | total 40 / 39 | D / F | Pass |
| U-08 | calculateGrade | total 0 and 100 | F and A+ | Pass |
| U-09 | readInt | letters instead of a number | "Invalid input", asks again | Pass |
| U-10 | studentExists | existing / missing ID | true / false | Pass |

## 3. Integration tests (modules working together)

| ID | Scenario | Expected | Result |
|----|----------|----------|--------|
| I-01 | Add a student, then add attendance for that student and a course | Saved, percentage shown | Pass |
| I-02 | Add attendance for a student ID that does not exist | "Student ID not found. Add the student first." | Pass |
| I-03 | Add attendance / result for a course code that does not exist | "Course code not found. Add the course first." | Pass |
| I-04 | Add a fee record for a missing student | "Student ID not found." | Pass |
| I-05 | Add student, faculty and course, then open the dashboard | Counts 1, 1, 1 | Pass |
| I-06 | Delete the only student, then open the dashboard | Total Students 0 | Pass |

## 4. System tests (complete features)

| ID | Feature | Test | Expected | Result |
|----|---------|------|----------|--------|
| S-01 | Login | Correct credentials | Main menu opens | Pass |
| S-02 | Login | Wrong credentials 3 times | Attempts counter 2, 1, 0, then "Too many failed login attempts" | Pass |
| S-03 | Student | Add, View, Search | Record saved, listed and found | Pass |
| S-04 | Student | Update an existing ID | "Student updated successfully", new values shown | Pass |
| S-05 | Student | Delete an existing ID | "Student deleted successfully", list empty | Pass |
| S-06 | Student | Add an existing ID | "already exists" | Pass |
| S-07 | Faculty | Add, View, Search | Record saved, listed and found | Pass |
| S-08 | Faculty | Add an existing ID | "already exists" | Pass |
| S-09 | Course | Add and View | Courses listed in a table | Pass |
| S-10 | Course | Add an existing code | "already exists" | Pass |
| S-11 | Attendance | 39 of 45 classes | 86.67%, no warning | Pass |
| S-12 | Attendance | 30 of 50 classes | 60.00% and warning below 75% | Pass |
| S-13 | Attendance | Attended more than total (45 of 40) | "Invalid attendance values." | Pass |
| S-14 | Result | Internal 38, external 55 | Total 93.00, grade A+ | Pass |
| S-15 | Result | Internal 25, external 30 | Total 55.00, grade C | Pass |
| S-16 | Result | Internal 12, external 20 | Total 32.00, grade F | Pass |
| S-17 | Result | Internal 78, external 89 | "Invalid marks" and nothing saved | Pass |
| S-18 | Fee | Total 75000, paid 45000 | Due 30000.00, status DUE | Pass |
| S-19 | Fee | Total 60000, paid 60000 | Due 0.00, status PAID | Pass |
| S-20 | Fee | Paid more than total | "Invalid fee values." | Pass |
| S-21 | Menu | Option that does not exist (3 in Result menu) | "Invalid choice." | Pass |
| S-22 | Exit | Choose 0 in the main menu | Thank-you message, program ends | Pass |
| S-23 | Persistence | Close and reopen the program | Saved records still listed | Pass |

## 5. Driver tests

### 5.1 Tests already executed

| ID | Test | Expected | Result |
|----|------|----------|--------|
| D-01 | Driver logic in a user-space harness (the driver source is compiled against stand-ins for the kernel functions): two writes, read back, EOF on second read, `ioctl` count and bytes used, clear, message truncated to 256 bytes, log fills to 8192 bytes then `-ENOSPC`, unknown `ioctl` and wrong magic number give `-ENOTTY` | 17 checks all correct | Pass (17 of 17) |
| D-02 | Application events written to a file standing in for the device (`CCMS_DEVICE`) | Login, student, course, attendance and exit lines with time stamps | Pass |
| D-03 | Application started while the device does not exist | Calculations and menus work; menu 8 shows how to load the driver | Pass |
| D-04 | Application compiled with the Linux code switched off (as on Windows) | Builds; menu 8 says the driver needs Linux | Pass |

The harness checks the driver's own logic. It is not a kernel run, so it does not prove that the module compiles
against a particular kernel or loads into it. That is covered by the tests below.

### 5.2 Tests to run on a Linux machine (record the result and add a screenshot)

| ID | Test | Expected | Result |
|----|------|----------|--------|
| D-05 | `make -C driver` | `ccms_log.ko` is produced without errors | |
| D-06 | `sudo insmod driver/ccms_log.ko` | No error; `dmesg` shows "ccms_log: loaded" | |
| D-07 | `ls -l /dev/ccms_log` | Device node exists | |
| D-08 | `echo "hello" > /dev/ccms_log` then `cat /dev/ccms_log` | `hello` is printed | |
| D-09 | Run the application, add a student, open menu 8 option 1 | Event lines appear in the log | |
| D-10 | Menu 8 option 2 | Event count and bytes used are shown (ioctl) | |
| D-11 | Menu 8 option 3, then option 1 | Log cleared; "No events logged yet." | |
| D-12 | `sudo rmmod ccms_log` | `/dev/ccms_log` disappears; `dmesg` shows "unloaded" | |

## 6. Defects found and fixed

| Defect | Fix |
|--------|-----|
| Duplicate IDs accepted | Duplicate checks added |
| Records accepted for students/courses that do not exist | Existence checks added |
| `cls` error message when built for a non-Windows system | OS-aware `clearScreen()` |

## 7. Code quality and reliability improvements
* Compiles with `gcc -Wall -Wextra` with zero warnings.
* Every file open is checked for failure.
* Input helper functions prevent crashes on non-numeric input.
* Common validation moved into helper functions instead of repeating code.

## 8. Known limitations found during testing
* Only Student supports Update and Delete.
* Credentials are fixed in the source.
* Data files are not encrypted.
