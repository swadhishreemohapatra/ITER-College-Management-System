# College Management System – System Architecture

## 1. Architecture Overview

The College Management System follows a layered software architecture implemented in C. The system consists of a Presentation Layer, Application Logic Layer, Data Storage Layer, and Linux Device Driver Interface.

```text
                 +---------------------------+
                 |       USER / ADMIN        |
                 |    Username + Password    |
                 +-------------+-------------+
                               |
                               v
                 +---------------------------+
                 |     PRESENTATION LAYER    |
                 |                           |
                 |  • Login                  |
                 |  • Main Menu              |
                 |  • Dashboard              |
                 |  • Console Input/Output   |
                 +-------------+-------------+
                               |
                               v
                 +---------------------------+
                 |   APPLICATION LOGIC LAYER |
                 |                           |
                 |  • Student Management     |
                 |  • Faculty Management     |
                 |  • Course Management      |
                 |  • Attendance Management  |
                 |  • Result Management      |
                 |  • Fee Management         |
                 |  • Validation             |
                 +-------------+-------------+
                               |
                  +------------+------------+
                  |                         |
                  v                         v
        +-------------------+     +----------------------+
        |   DATA STORAGE    |     | LINUX DRIVER LAYER  |
        |                   |     |                      |
        | students.dat      |     | Character Device     |
        | faculty.dat       |     | /dev/ccms_log        |
        | courses.dat       |     |                      |
        | attendance.dat    |     | open() / read()      |
        | results.dat       |     | write() / ioctl()    |
        | fees.dat          |     |                      |
        +-------------------+     +----------+-----------+
                                             |
                                             v
                                  +----------------------+
                                  |     LINUX KERNEL     |
                                  | Character Device     |
                                  | Driver Interface     |
                                  +----------------------+
