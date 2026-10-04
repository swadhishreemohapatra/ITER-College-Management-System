/*
 * Institute Of Technical Education And Research, Bhubaneswar
 * College Management System
 *
 * Single-file C project: main.c
 * Features:
 *   1. Admin Login
 *   2. Student Management
 *   3. Faculty Management
 *   4. Course Management
 *   5. Attendance Management
 *   6. Marks / Result Management
 *   7. Fee Management
 *
 * Data is stored in binary files in the same folder:
 * students.dat, faculty.dat, courses.dat,
 * attendance.dat, results.dat, fees.dat
 *
 * Default login:
 * Username: admin
 * Password: admin123
 *
 * Compile:
 *   gcc main.c -o college_management
 *
 * Run:
 *   Windows: college_management.exe
 *   Linux/macOS: ./college_management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STUDENT_FILE    "students.dat"
#define FACULTY_FILE    "faculty.dat"
#define COURSE_FILE     "courses.dat"
#define ATTENDANCE_FILE "attendance.dat"
#define RESULT_FILE     "results.dat"
#define FEE_FILE        "fees.dat"

#define MAX_NAME 80
#define MAX_DEPT 60
#define MAX_PHONE 20
#define MAX_EMAIL 80

typedef struct {
    int id;
    char name[MAX_NAME];
    char gender[15];
    int age;
    char department[MAX_DEPT];
    char course[60];
    int year;
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
} Student;

typedef struct {
    int id;
    char name[MAX_NAME];
    char department[MAX_DEPT];
    char designation[50];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
} Faculty;

typedef struct {
    int code;
    char name[80];
    char department[MAX_DEPT];
    int credits;
} Course;

typedef struct {
    int studentId;
    int courseCode;
    int totalClasses;
    int attendedClasses;
} Attendance;

typedef struct {
    int studentId;
    int courseCode;
    float internalMarks;
    float externalMarks;
    float totalMarks;
    char grade[5];
} Result;

typedef struct {
    int studentId;
    float totalFee;
    float paidFee;
    float dueFee;
    char status[20];
} Fee;

/* ---------- Utility Functions ---------- */

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);

    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int readInt(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
    }
}

float readFloat(const char *prompt) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1) {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
    }
}

void pauseScreen(void) {
    printf("\nPress ENTER to continue...");
    getchar();
}

void printHeader(const char *title) {
    printf("\n============================================================\n");
    printf(" INSTITUTE OF TECHNICAL EDUCATION AND RESEARCH, BHUBANESWAR\n");
    printf("                 COLLEGE MANAGEMENT SYSTEM\n");
    printf("============================================================\n");
    printf("                    %s\n", title);
    printf("============================================================\n");
}

void pressAndClear(void) {
    pauseScreen();
    system("cls");
#ifdef __linux__
    system("clear");
#endif
}

/* ---------- Login ---------- */

int login(void) {
    char username[50];
    char password[50];
    int attempts = 3;

    while (attempts > 0) {
        printHeader("ADMIN LOGIN");

        readLine("Username: ", username, sizeof(username));
        readLine("Password: ", password, sizeof(password));

        if (strcmp(username, "admin") == 0 &&
            strcmp(password, "admin123") == 0) {
            printf("\nLogin successful. Welcome, Administrator!\n");
            return 1;
        }

        attempts--;
        printf("\nInvalid username or password.");
        printf("\nAttempts remaining: %d\n", attempts);
        pauseScreen();
        system("cls");
#ifdef __linux__
        system("clear");
#endif
    }

    printf("\nToo many failed login attempts.\n");
    return 0;
}

/* ---------- Student Management ---------- */

void addStudent(void) {
    Student s;
    FILE *fp;

    printHeader("ADD STUDENT");

    s.id = readInt("Student ID: ");
    readLine("Student Name: ", s.name, sizeof(s.name));
    readLine("Gender: ", s.gender, sizeof(s.gender));
    s.age = readInt("Age: ");
    readLine("Department: ", s.department, sizeof(s.department));
    readLine("Course: ", s.course, sizeof(s.course));
    s.year = readInt("Year of Study: ");
    readLine("Phone: ", s.phone, sizeof(s.phone));
    readLine("Email: ", s.email, sizeof(s.email));

    fp = fopen(STUDENT_FILE, "ab");
    if (fp == NULL) {
        printf("\nError opening student database.\n");
        pauseScreen();
        return;
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("\nStudent added successfully.\n");
    pauseScreen();
}

void viewStudents(void) {
    Student s;
    FILE *fp;
    int count = 0;

    printHeader("ALL STUDENTS");

    fp = fopen(STUDENT_FILE, "rb");
    if (fp == NULL) {
        printf("No student records found.\n");
        pauseScreen();
        return;
    }

    printf("%-8s %-25s %-15s %-5s %-20s %-8s\n",
           "ID", "Name", "Department", "Age", "Course", "Year");
    printf("-------------------------------------------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-8d %-25s %-15s %-5d %-20s %-8d\n",
               s.id, s.name, s.department, s.age, s.course, s.year);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("No student records found.\n");

    pauseScreen();
}

void searchStudent(void) {
    Student s;
    FILE *fp;
    int id, found = 0;

    printHeader("SEARCH STUDENT");

    id = readInt("Enter Student ID: ");

    fp = fopen(STUDENT_FILE, "rb");
    if (fp == NULL) {
        printf("\nNo student records found.\n");
        pauseScreen();
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            printf("\nStudent ID     : %d", s.id);
            printf("\nName           : %s", s.name);
            printf("\nGender         : %s", s.gender);
            printf("\nAge            : %d", s.age);
            printf("\nDepartment     : %s", s.department);
            printf("\nCourse         : %s", s.course);
            printf("\nYear           : %d", s.year);
            printf("\nPhone          : %s", s.phone);
            printf("\nEmail          : %s\n", s.email);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nStudent not found.\n");

    pauseScreen();
}

void updateStudent(void) {
    Student s;
    FILE *fp;
    int id, found = 0;

    printHeader("UPDATE STUDENT");

    id = readInt("Enter Student ID to update: ");

    fp = fopen(STUDENT_FILE, "rb+");
    if (fp == NULL) {
        printf("\nNo student records found.\n");
        pauseScreen();
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            printf("\nEnter new information:\n");

            readLine("Student Name: ", s.name, sizeof(s.name));
            readLine("Gender: ", s.gender, sizeof(s.gender));
            s.age = readInt("Age: ");
            readLine("Department: ", s.department, sizeof(s.department));
            readLine("Course: ", s.course, sizeof(s.course));
            s.year = readInt("Year of Study: ");
            readLine("Phone: ", s.phone, sizeof(s.phone));
            readLine("Email: ", s.email, sizeof(s.email));

            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);

            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
        printf("\nStudent updated successfully.\n");
    else
        printf("\nStudent not found.\n");

    pauseScreen();
}

void deleteStudent(void) {
    Student s;
    FILE *fp, *temp;
    int id, found = 0;

    printHeader("DELETE STUDENT");

    id = readInt("Enter Student ID to delete: ");

    fp = fopen(STUDENT_FILE, "rb");
    temp = fopen("students_temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("\nUnable to access student database.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        pauseScreen();
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("students_temp.dat", STUDENT_FILE);

    if (found)
        printf("\nStudent deleted successfully.\n");
    else
        printf("\nStudent not found.\n");

    pauseScreen();
}

void studentMenu(void) {
    int choice;

    do {
        printHeader("STUDENT MANAGEMENT");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("0. Back to Main Menu\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: break;
            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Faculty Management ---------- */

void addFaculty(void) {
    Faculty f;
    FILE *fp;

    printHeader("ADD FACULTY");

    f.id = readInt("Faculty ID: ");
    readLine("Faculty Name: ", f.name, sizeof(f.name));
    readLine("Department: ", f.department, sizeof(f.department));
    readLine("Designation: ", f.designation, sizeof(f.designation));
    readLine("Phone: ", f.phone, sizeof(f.phone));
    readLine("Email: ", f.email, sizeof(f.email));

    fp = fopen(FACULTY_FILE, "ab");
    if (fp == NULL) {
        printf("\nError opening faculty database.\n");
        pauseScreen();
        return;
    }

    fwrite(&f, sizeof(Faculty), 1, fp);
    fclose(fp);

    printf("\nFaculty added successfully.\n");
    pauseScreen();
}

void viewFaculty(void) {
    Faculty f;
    FILE *fp;

    printHeader("ALL FACULTY");

    fp = fopen(FACULTY_FILE, "rb");
    if (fp == NULL) {
        printf("No faculty records found.\n");
        pauseScreen();
        return;
    }

    printf("%-8s %-25s %-20s %-18s\n",
           "ID", "Name", "Department", "Designation");
    printf("-----------------------------------------------------------------------\n");

    while (fread(&f, sizeof(Faculty), 1, fp) == 1) {
        printf("%-8d %-25s %-20s %-18s\n",
               f.id, f.name, f.department, f.designation);
    }

    fclose(fp);
    pauseScreen();
}

void searchFaculty(void) {
    Faculty f;
    FILE *fp;
    int id, found = 0;

    printHeader("SEARCH FACULTY");

    id = readInt("Enter Faculty ID: ");

    fp = fopen(FACULTY_FILE, "rb");
    if (fp == NULL) {
        printf("\nNo faculty records found.\n");
        pauseScreen();
        return;
    }

    while (fread(&f, sizeof(Faculty), 1, fp) == 1) {
        if (f.id == id) {
            printf("\nFaculty ID     : %d", f.id);
            printf("\nName           : %s", f.name);
            printf("\nDepartment     : %s", f.department);
            printf("\nDesignation    : %s", f.designation);
            printf("\nPhone          : %s", f.phone);
            printf("\nEmail          : %s\n", f.email);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\nFaculty not found.\n");

    pauseScreen();
}

void facultyMenu(void) {
    int choice;

    do {
        printHeader("FACULTY MANAGEMENT");
        printf("1. Add Faculty\n");
        printf("2. View All Faculty\n");
        printf("3. Search Faculty\n");
        printf("0. Back to Main Menu\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: addFaculty(); break;
            case 2: viewFaculty(); break;
            case 3: searchFaculty(); break;
            case 0: break;
            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Course Management ---------- */

void addCourse(void) {
    Course c;
    FILE *fp;

    printHeader("ADD COURSE");

    c.code = readInt("Course Code: ");
    readLine("Course Name: ", c.name, sizeof(c.name));
    readLine("Department: ", c.department, sizeof(c.department));
    c.credits = readInt("Credits: ");

    fp = fopen(COURSE_FILE, "ab");
    if (fp == NULL) {
        printf("\nError opening course database.\n");
        pauseScreen();
        return;
    }

    fwrite(&c, sizeof(Course), 1, fp);
    fclose(fp);

    printf("\nCourse added successfully.\n");
    pauseScreen();
}

void viewCourses(void) {
    Course c;
    FILE *fp;

    printHeader("ALL COURSES");

    fp = fopen(COURSE_FILE, "rb");
    if (fp == NULL) {
        printf("No course records found.\n");
        pauseScreen();
        return;
    }

    printf("%-10s %-30s %-20s %-8s\n",
           "Code", "Course Name", "Department", "Credits");
    printf("--------------------------------------------------------------------------\n");

    while (fread(&c, sizeof(Course), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %-8d\n",
               c.code, c.name, c.department, c.credits);
    }

    fclose(fp);
    pauseScreen();
}

void courseMenu(void) {
    int choice;

    do {
        printHeader("COURSE MANAGEMENT");
        printf("1. Add Course\n");
        printf("2. View Courses\n");
        printf("0. Back to Main Menu\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: addCourse(); break;
            case 2: viewCourses(); break;
            case 0: break;
            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Attendance Management ---------- */

void manageAttendance(void) {
    Attendance a;
    FILE *fp;
    float percentage;

    printHeader("ATTENDANCE MANAGEMENT");

    a.studentId = readInt("Student ID: ");
    a.courseCode = readInt("Course Code: ");
    a.totalClasses = readInt("Total Classes: ");
    a.attendedClasses = readInt("Classes Attended: ");

    if (a.totalClasses <= 0 ||
        a.attendedClasses < 0 ||
        a.attendedClasses > a.totalClasses) {
        printf("\nInvalid attendance values.\n");
        pauseScreen();
        return;
    }

    fp = fopen(ATTENDANCE_FILE, "ab");
    if (fp == NULL) {
        printf("\nError opening attendance database.\n");
        pauseScreen();
        return;
    }

    fwrite(&a, sizeof(Attendance), 1, fp);
    fclose(fp);

    percentage = ((float)a.attendedClasses / a.totalClasses) * 100.0f;

    printf("\nAttendance saved successfully.");
    printf("\nAttendance Percentage: %.2f%%\n", percentage);

    if (percentage < 75.0f)
        printf("Warning: Attendance is below 75%%.\n");

    pauseScreen();
}

void viewAttendance(void) {
    Attendance a;
    FILE *fp;
    float percentage;

    printHeader("ATTENDANCE REPORT");

    fp = fopen(ATTENDANCE_FILE, "rb");
    if (fp == NULL) {
        printf("No attendance records found.\n");
        pauseScreen();
        return;
    }

    printf("%-10s %-12s %-15s %-15s %-12s\n",
           "Student", "Course", "Total Classes",
           "Attended", "Percentage");
    printf("---------------------------------------------------------------------\n");

    while (fread(&a, sizeof(Attendance), 1, fp) == 1) {
        percentage = ((float)a.attendedClasses / a.totalClasses) * 100.0f;

        printf("%-10d %-12d %-15d %-15d %.2f%%\n",
               a.studentId,
               a.courseCode,
               a.totalClasses,
               a.attendedClasses,
               percentage);
    }

    fclose(fp);
    pauseScreen();
}

void attendanceMenu(void) {
    int choice;

    do {
        printHeader("ATTENDANCE MANAGEMENT");
        printf("1. Add Attendance\n");
        printf("2. View Attendance Report\n");
        printf("0. Back to Main Menu\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: manageAttendance(); break;
            case 2: viewAttendance(); break;
            case 0: break;
            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Result Management ---------- */

const char *calculateGrade(float total) {
    if (total >= 90) return "A+";
    if (total >= 80) return "A";
    if (total >= 70) return "B+";
    if (total >= 60) return "B";
    if (total >= 50) return "C";
    if (total >= 40) return "D";
    return "F";
}

void addResult(void) {
    Result r;
    FILE *fp;

    printHeader("ADD RESULT");

    r.studentId = readInt("Student ID: ");
    r.courseCode = readInt("Course Code: ");
    r.internalMarks = readFloat("Internal Marks (0-40): ");
    r.externalMarks = readFloat("External Marks (0-60): ");

    if (r.internalMarks < 0 || r.internalMarks > 40 ||
        r.externalMarks < 0 || r.externalMarks > 60) {
        printf("\nInvalid marks. Internal must be 0-40 and external 0-60.\n");
        pauseScreen();
        return;
    }

    r.totalMarks = r.internalMarks + r.externalMarks;
    strcpy(r.grade, calculateGrade(r.totalMarks));

    fp = fopen(RESULT_FILE, "ab");
    if (fp == NULL) {
        printf("\nError opening result database.\n");
        pauseScreen();
        return;
    }

    fwrite(&r, sizeof(Result), 1, fp);
    fclose(fp);

    printf("\nResult saved successfully.");
    printf("\nTotal Marks: %.2f", r.totalMarks);
    printf("\nGrade      : %s\n", r.grade);

    pauseScreen();
}

void viewResults(void) {
    Result r;
    FILE *fp;

    printHeader("RESULT REPORT");

    fp = fopen(RESULT_FILE, "rb");
    if (fp == NULL) {
        printf("No result records found.\n");
        pauseScreen();
        return;
    }

    printf("%-10s %-10s %-15s %-15s %-12s %-8s\n",
           "Student", "Course", "Internal", "External",
           "Total", "Grade");
    printf("------------------------------------------------------------------------\n");

    while (fread(&r, sizeof(Result), 1, fp) == 1) {
        printf("%-10d %-10d %-15.2f %-15.2f %-12.2f %-8s\n",
               r.studentId,
               r.courseCode,
               r.internalMarks,
               r.externalMarks,
               r.totalMarks,
               r.grade);
    }

    fclose(fp);
    pauseScreen();
}

void resultMenu(void) {
    int choice;

    do {
        printHeader("RESULT MANAGEMENT");
        printf("1. Add Result\n");
        printf("2. View Results\n");
        printf("0. Back to Main Menu\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: addResult(); break;
            case 2: viewResults(); break;
            case 0: break;
            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Fee Management ---------- */

void addFee(void) {
    Fee f;
    FILE *fp;

    printHeader("FEE MANAGEMENT");

    f.studentId = readInt("Student ID: ");
    f.totalFee = readFloat("Total Fee: ");
    f.paidFee = readFloat("Paid Fee: ");

    if (f.totalFee < 0 || f.paidFee < 0 || f.paidFee > f.totalFee) {
        printf("\nInvalid fee values.\n");
        pauseScreen();
        return;
    }

    f.dueFee = f.totalFee - f.paidFee;

    if (f.dueFee <= 0)
        strcpy(f.status, "PAID");
    else
        strcpy(f.status, "DUE");

    fp = fopen(FEE_FILE, "ab");
    if (fp == NULL) {
        printf("\nError opening fee database.\n");
        pauseScreen();
        return;
    }

    fwrite(&f, sizeof(Fee), 1, fp);
    fclose(fp);

    printf("\nFee record saved successfully.");
    printf("\nTotal Fee : %.2f", f.totalFee);
    printf("\nPaid Fee  : %.2f", f.paidFee);
    printf("\nDue Fee   : %.2f", f.dueFee);
    printf("\nStatus    : %s\n", f.status);

    pauseScreen();
}

void viewFees(void) {
    Fee f;
    FILE *fp;

    printHeader("FEE REPORT");

    fp = fopen(FEE_FILE, "rb");
    if (fp == NULL) {
        printf("No fee records found.\n");
        pauseScreen();
        return;
    }

    printf("%-10s %-15s %-15s %-15s %-10s\n",
           "Student", "Total Fee", "Paid Fee", "Due Fee", "Status");
    printf("----------------------------------------------------------------\n");

    while (fread(&f, sizeof(Fee), 1, fp) == 1) {
        printf("%-10d %-15.2f %-15.2f %-15.2f %-10s\n",
               f.studentId,
               f.totalFee,
               f.paidFee,
               f.dueFee,
               f.status);
    }

    fclose(fp);
    pauseScreen();
}

void feeMenu(void) {
    int choice;

    do {
        printHeader("FEE MANAGEMENT");
        printf("1. Add Fee Record\n");
        printf("2. View Fee Report\n");
        printf("0. Back to Main Menu\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: addFee(); break;
            case 2: viewFees(); break;
            case 0: break;
            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Dashboard ---------- */

void dashboard(void) {
    FILE *fp;
    Student s;
    Faculty f;
    Course c;
    int students = 0, faculty = 0, courses = 0;

    printHeader("COLLEGE DASHBOARD");

    fp = fopen(STUDENT_FILE, "rb");
    if (fp != NULL) {
        while (fread(&s, sizeof(Student), 1, fp) == 1)
            students++;
        fclose(fp);
    }

    fp = fopen(FACULTY_FILE, "rb");
    if (fp != NULL) {
        while (fread(&f, sizeof(Faculty), 1, fp) == 1)
            faculty++;
        fclose(fp);
    }

    fp = fopen(COURSE_FILE, "rb");
    if (fp != NULL) {
        while (fread(&c, sizeof(Course), 1, fp) == 1)
            courses++;
        fclose(fp);
    }

    printf("\n              COLLEGE STATISTICS\n");
    printf("              ------------------\n");
    printf("              Total Students : %d\n", students);
    printf("              Total Faculty  : %d\n", faculty);
    printf("              Total Courses  : %d\n", courses);

    pauseScreen();
}

/* ---------- Main Menu ---------- */

void mainMenu(void) {
    int choice;

    do {
        printHeader("MAIN MENU");

        printf("1.  Student Management\n");
        printf("2.  Faculty Management\n");
        printf("3.  Course Management\n");
        printf("4.  Attendance Management\n");
        printf("5.  Result / Marks Management\n");
        printf("6.  Fee Management\n");
        printf("7.  College Dashboard\n");
        printf("0.  Exit\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1:
                studentMenu();
                break;
            case 2:
                facultyMenu();
                break;
            case 3:
                courseMenu();
                break;
            case 4:
                attendanceMenu();
                break;
            case 5:
                resultMenu();
                break;
            case 6:
                feeMenu();
                break;
            case 7:
                dashboard();
                break;
            case 0:
                printf("\nThank you for using Institute Of Technical Education And Research, Bhubaneswar\n");
                printf("College Management System.\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pauseScreen();
        }
    } while (choice != 0);
}

/* ---------- Program Entry ---------- */

int main(void) {
    if (!login()) {
        return 0;
    }

    system("cls");
#ifdef __linux__
    system("clear");
#endif

    mainMenu();

    return 0;
}
