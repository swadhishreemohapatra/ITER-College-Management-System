# Stage 3 - System Design and Architecture

## 1. High-level architecture

The program has three layers: a console interface, the application logic (one set of functions per module)
and a file-based storage layer.

```mermaid
flowchart TD
    A[Administrator] --> B[Console Interface<br/>menus and prompts]
    B --> C[Login]
    C --> D[Main Menu]
    D --> S[Student Module]
    D --> F[Faculty Module]
    D --> CO[Course Module]
    D --> AT[Attendance Module]
    D --> R[Result Module]
    D --> FE[Fee Module]
    D --> DB[Dashboard]
    D --> DM[Driver Event Log menu]
    DM --> K[["/dev/ccms_log<br/>Linux kernel driver"]]
    S -. log events .-> K
    AT -. log events .-> K
    R -. log events .-> K
    FE -. log events .-> K
    S --> V[Validation Helpers<br/>studentExists, facultyExists, courseExists]
    F --> V
    CO --> V
    AT --> V
    R --> V
    FE --> V
    S --> P[(students.dat)]
    F --> Q[(faculty.dat)]
    CO --> T[(courses.dat)]
    AT --> U[(attendance.dat)]
    R --> W[(results.dat)]
    FE --> X[(fees.dat)]
    DB --> P
    DB --> Q
    DB --> T
```

## 2. Components and responsibilities

| Component | Responsibility |
|-----------|----------------|
| Input utilities | Read validated integers, floats and text lines; pause and clear the screen; print the header |
| Login | Check username and password with 3 attempts |
| Validation helpers | Check whether a student, faculty member or course already exists |
| Student / Faculty / Course modules | Maintain the master records |
| Attendance / Result / Fee modules | Maintain records that depend on a student and a course |
| Dashboard | Count records in three files |
| Driver interface | `logEvent` writes events to `/dev/ccms_log`; menu 8 reads the log and calls `ioctl` |
| Kernel driver (`ccms_log`) | Owns the log buffer in kernel memory and serves `open`, `read`, `write`, `ioctl` |
| Main menu | Route the administrator to a module |

## 3. Data structures

```c
typedef struct { int id; char name[80]; char gender[15]; int age;
                 char department[60]; char course[60]; int year;
                 char phone[20]; char email[80]; } Student;

typedef struct { int id; char name[80]; char department[60];
                 char designation[50]; char phone[20]; char email[80]; } Faculty;

typedef struct { int code; char name[80]; char department[60]; int credits; } Course;

typedef struct { int studentId; int courseCode; int totalClasses; int attendedClasses; } Attendance;

typedef struct { int studentId; int courseCode; float internalMarks; float externalMarks;
                 float totalMarks; char grade[5]; } Result;

typedef struct { int studentId; float totalFee; float paidFee; float dueFee; char status[20]; } Fee;
```

Each record is one `struct`; a file is an array of fixed-size records written with `fwrite`.

## 4. UML diagrams

### 4.1 Class (data model) diagram

```mermaid
classDiagram
    class Student {
        +int id
        +char name
        +char gender
        +int age
        +char department
        +char course
        +int year
        +char phone
        +char email
    }
    class Faculty {
        +int id
        +char name
        +char department
        +char designation
        +char phone
        +char email
    }
    class Course {
        +int code
        +char name
        +char department
        +int credits
    }
    class Attendance {
        +int studentId
        +int courseCode
        +int totalClasses
        +int attendedClasses
    }
    class Result {
        +int studentId
        +int courseCode
        +float internalMarks
        +float externalMarks
        +float totalMarks
        +char grade
    }
    class Fee {
        +int studentId
        +float totalFee
        +float paidFee
        +float dueFee
        +char status
    }
    Student "1" --> "*" Attendance : has
    Course "1" --> "*" Attendance : for
    Student "1" --> "*" Result : earns
    Course "1" --> "*" Result : for
    Student "1" --> "*" Fee : pays
```

### 4.2 Sequence diagram - Add Student

```mermaid
sequenceDiagram
    actor Admin
    participant Menu as studentMenu()
    participant Add as addStudent()
    participant Chk as studentExists()
    participant File as students.dat
    Admin->>Menu: choose 1 (Add Student)
    Menu->>Add: call
    Add->>Admin: ask Student ID
    Admin->>Add: enter ID
    Add->>Chk: studentExists(id)
    Chk->>File: read records
    File-->>Chk: records
    alt ID already exists
        Chk-->>Add: true
        Add->>Admin: "already exists"
    else new ID
        Chk-->>Add: false
        Add->>Admin: ask remaining details
        Admin->>Add: enter details
        Add->>File: fwrite(record)
        Add->>Admin: "added successfully"
    end
```

### 4.3 State machine diagram - program navigation

```mermaid
stateDiagram-v2
    [*] --> Login
    Login --> MainMenu: correct credentials
    Login --> Login: wrong credentials (attempts left)
    Login --> [*]: 3 failed attempts
    MainMenu --> ModuleMenu: choose 1 to 6
    MainMenu --> Dashboard: choose 7
    Dashboard --> MainMenu: press Enter
    ModuleMenu --> Operation: choose an operation
    Operation --> ModuleMenu: operation finished
    ModuleMenu --> MainMenu: choose 0
    MainMenu --> [*]: choose 0 (Exit)
```

## 5. System programming concepts used

* **File handling:** binary files opened with `fopen` in `ab`, `rb`, `rb+` and `wb` modes; `fread`, `fwrite`, `fseek` for in-place update.
* **Delete operation:** copy the records to keep into a temporary file, then `remove` the original and `rename` the temporary file.
* **Process interaction:** `system()` is used to clear the console.
* **Input handling:** buffer clearing and validation to keep the standard input stream consistent.

## 6. Implementation plan
Build in this order: utilities and login, student module, faculty, course, attendance, result, fee,
dashboard, main menu, then validation.

## 7. Development environment

| Item | Choice |
|------|--------|
| Operating system | Windows 11 |
| Compiler | GCC, installed through MSYS2 UCRT64 |
| Editor | Any text editor or Visual Studio Code |
| Version control | Git with a GitHub repository |
| Build command | `gcc src/main.c -o college_management` |

## 8. Git repository and branching strategy

* `main` branch: always holds a working version.
* `dev` branch: day-to-day work; merged into `main` when a stage is complete.
* One commit per finished feature with a clear message, for example `Add duplicate ID check for students`.
* Tags at the end of each stage: `stage1` ... `stage6`.

## 9. Documentation and progress tracking
One document per stage in the `docs` folder; progress and issues are recorded in `Stage4_Prototype_and_Progress.md`
and in the Git commit history.

## 10. Linux device driver design

### 10.1 Purpose
The driver adds a kernel-level component: a character device `/dev/ccms_log` that holds an audit trail of what
the administrator does. The application is the only writer; the administrator reads the log from menu 8 or with `cat`.

### 10.2 Driver concepts used

| Concept | Where |
|---------|-------|
| Loadable kernel module | `module_init(ccms_init)`, `module_exit(ccms_exit)`, `MODULE_LICENSE("GPL")` |
| Device number | `alloc_chrdev_region` / `unregister_chrdev_region` |
| Character device | `cdev_init`, `cdev_add`, `cdev_del` |
| Automatic device node | `class_create`, `device_create`, `device_destroy`, `class_destroy` |
| File operations | `struct file_operations`: `open`, `release`, `read`, `write`, `unlocked_ioctl` |
| Kernel/user data transfer | `copy_from_user`, `simple_read_from_buffer`, `put_user` |
| ioctl interface | commands built with `_IO` / `_IOR` in `ccms_ioctl.h` |
| Synchronization | `DEFINE_MUTEX`, `mutex_lock`, `mutex_unlock` |
| Error handling | `-ENOSPC`, `-EFAULT`, `-ENOTTY`, `goto` clean-up in `ccms_init` |
| Kernel logging | `pr_info`, visible with `dmesg` |

### 10.3 Driver interface

| Operation | Behaviour |
|-----------|-----------|
| `write` | Appends up to 256 bytes (one event). Returns `-ENOSPC` when the 8192-byte log is full. |
| `read` | Returns the stored log from the current file position; 0 at the end. |
| `ioctl CCMS_IOC_GET_COUNT` | Returns the number of events written. |
| `ioctl CCMS_IOC_GET_USED` | Returns the number of bytes used. |
| `ioctl CCMS_IOC_CLEAR` | Empties the log. |
| any other `ioctl` | Returns `-ENOTTY`. |

### 10.4 Sequence diagram - logging an event

```mermaid
sequenceDiagram
    actor Admin
    participant App as addStudent()
    participant Log as logEvent()
    participant VFS as Linux VFS
    participant Drv as ccms_write()
    participant Buf as log buffer (kernel)
    Admin->>App: add student
    App->>App: save record in students.dat
    App->>Log: logEvent("STUDENT_ADDED id=1")
    Log->>VFS: open("/dev/ccms_log"), write(line)
    VFS->>Drv: file_operations.write
    Drv->>Drv: mutex_lock
    Drv->>Buf: copy_from_user, append
    Drv->>Drv: event_count++, mutex_unlock
    Drv-->>Log: bytes written
    Log->>VFS: close
    App-->>Admin: "Student added successfully"
```

### 10.5 State machine - driver life cycle

```mermaid
stateDiagram-v2
    [*] --> Unloaded
    Unloaded --> Loaded: insmod (ccms_init creates /dev/ccms_log)
    Loaded --> Loaded: open / read / write / ioctl
    Loaded --> Loaded: ioctl CLEAR (log emptied)
    Loaded --> Unloaded: rmmod (ccms_exit removes /dev/ccms_log)
    Unloaded --> [*]
```

### 10.6 Design decisions
* The application ignores a missing driver, so the college features never depend on the kernel module.
* The log is kept in memory and is not persistent; the `.dat` files remain the permanent store.
* `CCMS_DEVICE` can point the application at another file, which allows testing without loading the module.
* The build, load and unload steps are scripted in the `scripts` folder.
