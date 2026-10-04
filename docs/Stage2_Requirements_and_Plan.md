# Stage 2 - Project Requirements Document (PRD) and Development Plan

## 1. Purpose
This document lists what the College Management System must do (functional requirements), how well it must
do it (non-functional requirements), what is included, and how the work is planned.

## 2. Functional requirements

| ID | Requirement |
|----|-------------|
| FR-01 | The system shall ask for a username and password and allow only 3 attempts. |
| FR-02 | The system shall add, view, search, update and delete student records. |
| FR-03 | The system shall add, view and search faculty records. |
| FR-04 | The system shall add and view course records. |
| FR-05 | The system shall record attendance (total and attended classes) per student and course, and calculate the percentage. |
| FR-06 | The system shall warn when attendance is below 75%. |
| FR-07 | The system shall record internal (0-40) and external (0-60) marks, calculate the total and assign a grade. |
| FR-08 | The system shall record total and paid fees, calculate the due amount and show PAID or DUE. |
| FR-09 | The system shall show a dashboard with the number of students, faculty and courses. |
| FR-10 | The system shall reject duplicate Student IDs, Faculty IDs and Course codes. |
| FR-11 | The system shall accept attendance, result and fee records only for existing students and courses. |
| FR-12 | The system shall keep all data in files so it is available after the program is closed. |
| FR-13 | On Linux, a kernel driver shall provide the device `/dev/ccms_log` that stores audit events written by the application. |
| FR-14 | The driver shall return the stored log on `read` and report the event count and bytes used through `ioctl`. |
| FR-15 | The driver shall allow the log to be cleared through `ioctl` and shall refuse writes when the log is full. |
| FR-16 | The application shall log login, add, update, delete and exit events to the driver and shall keep working when the driver is not present. |

### Grade scale (FR-07)

| Total marks | Grade |
|-------------|-------|
| 90 and above | A+ |
| 80 - 89 | A |
| 70 - 79 | B+ |
| 60 - 69 | B |
| 50 - 59 | C |
| 40 - 49 | D |
| below 40 | F |

## 3. Non-functional requirements

| ID | Requirement |
|----|-------------|
| NFR-01 | **Usability:** a clear numbered menu and plain messages for every success or error. |
| NFR-02 | **Reliability:** invalid input (letters instead of numbers, out-of-range values) must not crash the program. |
| NFR-03 | **Data integrity:** saved records must survive closing and reopening the program. |
| NFR-04 | **Portability:** standard C only, building with GCC without extra libraries. |
| NFR-05 | **Performance:** every operation responds instantly for the data sizes of a small college. |
| NFR-06 | **Maintainability:** one function per operation, grouped by module, with comments. |
| NFR-07 | **Security:** access only after login (limitation: fixed credentials, unencrypted files). |
| NFR-08 | **Driver safety:** all user memory is accessed with `copy_from_user` / `put_user`; the buffer is protected by a mutex; message size and total size are bounded. |

## 4. Modules and deliverables

| Module | Main functions | Data file |
|--------|----------------|-----------|
| Utilities | readInt, readFloat, readLine, printHeader, clearScreen | - |
| Login | login | - |
| Student | addStudent, viewStudents, searchStudent, updateStudent, deleteStudent | students.dat |
| Faculty | addFaculty, viewFaculty, searchFaculty | faculty.dat |
| Course | addCourse, viewCourses | courses.dat |
| Attendance | manageAttendance, viewAttendance | attendance.dat |
| Result | addResult, viewResults, calculateGrade | results.dat |
| Fee | addFee, viewFees | fees.dat |
| Dashboard | dashboard | reads three files |
| Driver interface | logEvent, viewDriverLog, showDriverStats, clearDriverLog | /dev/ccms_log |
| Kernel driver | ccms_open, ccms_release, ccms_read, ccms_write, ccms_ioctl, ccms_init, ccms_exit | log buffer in kernel memory |

**Deliverables:** source code, README, six stage documents, UML and architecture diagrams, test report,
GitHub repository, final presentation.

## 5. Development plan and timeline

| Stage | Work | Output |
|-------|------|--------|
| 1 | Idea, problem, scope | Stage 1 document |
| 2 | Requirements, plan | This document |
| 3 | Architecture, data structures, UML, environment, Git setup | Stage 3 document |
| 4 | Core modules and first prototype | Working menu with student, faculty, course modules |
| 5 | Remaining modules, validation, testing, fixes | Test report |
| 6 | Final build, demonstration, report | Final report and presentation |

The Linux device driver was added after the application prototype (Stages 4 and 5).

Milestones: prototype running, all seven modules working, validation added, documentation complete,
repository submitted by the capstone deadline (5 October 2026).

## 6. Risks

| Risk | Mitigation |
|------|------------|
| Invalid keyboard input | Input helper functions re-ask until a valid number is entered. |
| Data file missing or corrupt | Every file open is checked and a message is shown. |
| Short time before the deadline | Single-file design and modules built one at a time. |
