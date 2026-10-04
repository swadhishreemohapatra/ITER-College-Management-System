# Stage 1 - Project Introduction

## Project title
College Management System - Institute Of Technical Education And Research, Bhubaneswar

## Objective
To build a console application in C that lets a college administrator maintain student, faculty, course,
attendance, result and fee records in one place, replacing paper registers and scattered spreadsheets.

## Problem to be solved
Colleges keep several kinds of records that are closely related. When they are kept manually:

* records are slow to search and easy to lose,
* the same data is copied in many places and becomes inconsistent,
* totals (attendance percentage, grade, fee due) are calculated by hand and can be wrong,
* there is no single view of how many students, faculty and courses exist.

## Scope

**In scope**

* Administrator login
* Student, Faculty, Course, Attendance, Result and Fee modules
* Automatic calculation of attendance %, grade and fee due
* Dashboard with college statistics
* Persistent storage in binary files
* Menu-driven console interface
* A Linux character device driver (`/dev/ccms_log`) that keeps an audit log of application events, with `read`, `write` and `ioctl` support

**Out of scope**

* Graphical or web interface
* Multi-user roles and network access
* Database server
* Drivers for real hardware (the driver is a software character device)

## Expected outcome
A working, validated, menu-driven program that stores data between runs and reports attendance, results
and fees correctly.

## Applications
Small colleges and training institutes that need a lightweight record-keeping tool; also a teaching example
of structured C programming, file handling and modular design.

## Technology
C language, GCC compiler, Windows 11 with MSYS2 UCRT64 for the application, Linux (Ubuntu) with kernel headers for the driver, Git and GitHub for version control.
