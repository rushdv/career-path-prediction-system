#include <stdio.h>
#include <string.h>
#include "sha256.h"
#include "student.h"
#include "colors.h"
#include "db_handler.h"
#include "input_handler.h"

/* Implement SHA-256 password hashing */
void hashPassword(const char *input, char *output) {
    sha256_hash_string(input, output);
}

/* ── Create new student ─────────────────────────────────── */
void createStudent(void) {
    Student s;
    memset(&s, 0, sizeof(Student));

    CLEAR_SCREEN();
    printf("\n");
    printBanner("CREATE NEW STUDENT PROFILE", 26);
    printf("\n");

    inputPrompt("Full Name");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\r\n")] = 0;

    inputPrompt("Student ID");
    getStringInput(s.studentID, sizeof(s.studentID));

    /* Duplicate check */
    Student temp;
    if (db_get_student_by_id(s.studentID, &temp) && temp.isActive) {
        printError("Student ID already exists. Please try a different ID.");
        return;
    }

    char rawPass[65];
    inputPrompt("Password");
    printf("\033[8m"); /* conceal */
    getStringInput(rawPass, sizeof(rawPass));
    printf("\033[28m"); /* reveal */
    hashPassword(rawPass, s.password);

    inputPrompt("CGPA (0.0 - 4.0)");
    s.cgpa = getFloatInput(0.0f, 4.0f);

    inputPrompt("Department");
    fgets(s.department, sizeof(s.department), stdin);
    s.department[strcspn(s.department, "\n")] = 0;

    s.isActive = 1;

    if (db_insert_student(&s)) {
        db_get_student_by_id(s.studentID, &s); /* Fetch the auto-incremented ID */
        printf("\n");
        printSuccess("Student registered successfully!");
        char idStr[16];
        snprintf(idStr, sizeof(idStr), "%d", s.id);
        printKV("Unique ID",  idStr);
        printKV("Student ID", s.studentID);
        printf("\n");
    } else {
        printError("Failed to register student in the database.");
    }
}

/* ── View student profile ───────────────────────────────── */
void viewStudent(const char *studentID) {
    Student s;
    if (db_get_student_by_id(studentID, &s) && s.isActive) {
        printf("\n");
        printBanner("STUDENT PROFILE", 15);
        printf("\n");

        char idStr[12], cgpaStr[10];
        snprintf(idStr,   sizeof(idStr),   "%d",   s.id);
        snprintf(cgpaStr, sizeof(cgpaStr), "%.2f", s.cgpa);

        printKV("Name",       s.name);
        printKV("Student ID", s.studentID);
        printKV("Unique ID",  idStr);
        printKV("CGPA",       cgpaStr);
        printKV("Department", s.department);
        printf("\n");
    } else {
        printError("Student ID not found.");
    }
}

/* ── Update student profile ─────────────────────────────── */
void updateStudent(const char *studentID) {
    Student s;
    if (db_get_student_by_id(studentID, &s) && s.isActive) {
        printf("\n");
        printBanner("UPDATE STUDENT PROFILE", 22);
        printf("\n");

        printf(UI_PAD "%sLeave blank and press Enter to keep current value.%s\n\n",
               CC(C_DIM), CC(RESET));

        /* Name */
        printf(UI_PAD "%sCurrent Name       %s: %s%s%s\n",
               CC(C_INFO), CC(RESET), CC(C_VALUE), s.name, CC(RESET));
        inputPrompt("New Name");
        char tmp[50];
        fgets(tmp, sizeof(tmp), stdin);
        tmp[strcspn(tmp, "\r\n")] = 0;
        if (strlen(tmp) > 0) {
            strncpy(s.name, tmp, sizeof(s.name) - 1);
        }

        /* Password */
        printf(UI_PAD "%sCurrent Password   %s: %s********%s\n",
               CC(C_INFO), CC(RESET), CC(C_VALUE), CC(RESET));
        inputPrompt("New Password (or leave blank to skip)");
        char tmpPass[50];
        fgets(tmpPass, sizeof(tmpPass), stdin);
        tmpPass[strcspn(tmpPass, "\r\n")] = 0;
        if (strlen(tmpPass) > 0) {
            char oldPass[50];
            char oldHash[65];
            inputPrompt("Enter Current Password to confirm");
            printf("\033[8m"); /* conceal */
            fgets(oldPass, sizeof(oldPass), stdin);
            printf("\033[28m"); /* reveal */
            oldPass[strcspn(oldPass, "\r\n")] = 0;
            
            hashPassword(oldPass, oldHash);
            if (strcmp(s.password, oldHash) == 0) {
                hashPassword(tmpPass, s.password);
                printSuccess("Password updated.");
            } else {
                printError("Incorrect current password. Password NOT updated.");
            }
        }

        /* CGPA */
        printf(UI_PAD "%sCurrent CGPA       %s: %s%.2f%s\n",
               CC(C_INFO), CC(RESET), CC(C_VALUE), s.cgpa, CC(RESET));
        inputPrompt("New CGPA (or leave blank to skip)");
        char tmpCgpa[20];
        fgets(tmpCgpa, sizeof(tmpCgpa), stdin);
        tmpCgpa[strcspn(tmpCgpa, "\r\n")] = 0;
        if (strlen(tmpCgpa) > 0) {
            float newCgpa;
            if (sscanf(tmpCgpa, "%f", &newCgpa) == 1) {
                if (newCgpa < 0.0f || newCgpa > 4.0f) {
                    printError("CGPA must be between 0.0 and 4.0. Update aborted.");
                    pauseScreen();
                    return;
                }
                s.cgpa = newCgpa;
            }
        }

        /* Department */
        printf(UI_PAD "%sCurrent Department %s: %s%s%s\n",
               CC(C_INFO), CC(RESET), CC(C_VALUE), s.department, CC(RESET));
        inputPrompt("New Department");
        fgets(tmp, sizeof(tmp), stdin);
        tmp[strcspn(tmp, "\r\n")] = 0;
        if (strlen(tmp) > 0) {
            strncpy(s.department, tmp, sizeof(s.department) - 1);
        }

        if (db_update_student(&s)) {
            printf("\n");
            printSuccess("Profile updated successfully!");
        } else {
            printError("Failed to update profile in database.");
        }
    } else {
        printError("Student ID not found. Update failed.");
    }
}

/* ── Delete student (soft) ──────────────────────────────── */
void deleteStudent(const char *studentID) {
    Student s;
    if (db_get_student_by_id(studentID, &s) && s.isActive) {
        printf("\n" UI_PAD "%s%s  Are you sure you want to delete student %s%s%s%s? (y/n): %s",
               CC(C_WARNING), SYM_WARN,
               CC(RESET), CC(C_VALUE), CC(BOLD), s.name,
               CC(RESET));

        char confirm = (char)getchar();
        getchar();

        if (confirm != 'y' && confirm != 'Y') {
            printInfo("Deletion cancelled.");
            return;
        }

        if (db_delete_student(studentID)) {
            printSuccess("Student deleted successfully.");
        } else {
            printError("Database error during deletion.");
        }
    } else {
        printError("Student ID not found. Deletion failed.");
    }
}

/* ── List all active students ───────────────────────────── */
void listAllStudents(void) {
    printf("\n");
    printBanner("ALL ACTIVE STUDENTS", 19);
    printf("\n");
    
    printf(UI_PAD "%s%s%-4s  %-24s  %-12s  %-6s  %-14s%s\n",
           CC(C_PURPLE), CC(BOLD),
           "ID", "Name", "Student ID", "CGPA", "Department",
           CC(RESET));
    printf(UI_PAD "%s", CC(C_DIM));
    for (int j = 0; j < 68; j++) fputs(BOX_H, stdout);
    printf("%s\n", CC(RESET));

    db_list_all_students();
}

/* ── Search by name ─────────────────────────────────────── */
void searchByName(const char *name) {
    printf("\n");
    printBanner("SEARCH RESULTS", 14);
    printf("\n");
    db_search_students(name);
}
