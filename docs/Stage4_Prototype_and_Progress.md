# Stage 4 - Initial Implementation, Prototype and Progress

## Prototype
The first working prototype is a single C file (`src/main.c`) with the login screen, the main menu and all
seven modules. The prototype was compiled with GCC under MSYS2 UCRT64 on Windows 11 and every module was
demonstrated from the console.

## Progress log

| Date | Work done |
|------|-----------|
| 1 Oct 2026 | Capstone topic decided; development environment planned |
| 2 Oct 2026 | MSYS2 UCRT64 and GCC installed on Windows 11; project folder created; program compiled and run |
| 2 Oct 2026 | Student, Faculty, Course, Attendance, Result, Fee modules and Dashboard run and checked through the menus |
| 2 Oct 2026 | Institute name updated in the header and exit message |
| 3 Oct 2026 | Validation added (duplicate IDs, existing student/course checks); OS-aware clear-screen function; documentation written |
| 3 Oct 2026 | Linux character device driver `ccms_log` written (read, write, ioctl, mutex); application interface added (`logEvent`, menu 8); driver logic tested in a user-space harness |

## Issues found and solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| `cd` command failed to open the project folder | A placeholder user name was typed instead of the real Windows user folder | Used the correct path `/c/Users/<user>/OneDrive/Desktop/CollegeProject` |
| Old text remained visible in the MSYS2 window after clearing the screen | The MSYS2 terminal does not fully erase the screen | Display-only issue; data unaffected. Windows Command Prompt clears correctly |
| Two faculty records with the same ID were accepted | No duplicate check in the Add functions | Added `studentExists`, `facultyExists`, `courseExists` and checks in the Add functions |
| Attendance, result and fee accepted student IDs that did not exist | No reference check | Added existence checks before saving |
| `cls` message appeared when compiled for a non-Windows system | The clear-screen call was Windows-specific | Replaced with `clearScreen()` that chooses `cls` or `clear` by operating system |
| Driver cannot be loaded on Windows | A kernel module needs the Linux kernel | Application built to run normally without the driver; driver built and loaded on Linux only |

## Next stage
Complete testing of all modules, record results, fix defects and finish documentation.
