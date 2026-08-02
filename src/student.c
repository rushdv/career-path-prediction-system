#include <stdio.h>
#include <string.h>
#include "student.h"
#include "colors.h"
#include "file_handler.h"

/* Generate next unique numeric ID */
static int generateNextID(void) {
    Student arr[100];
    int n;
    loadAllStudents(arr, &n);
    if (n == 0) return 1;
    return arr[n - 1].id + 1;
}

/* ── Create new student ─────────────────────────────────── */
void createStudent(void) {
    Student s;

    CLEAR_SCREEN();
    printf("\n");
    printBanner("  CREATE NEW STUDENT PROFILE  ", 30);
    printf("\n");

    inputPrompt("Full Name");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    inputPrompt("Student ID");
    scanf("%s", s.studentID);
    getchar();

    /* Duplicate check */
    Student arr[100];
    int n, i;
    loadAllStudents(arr, &n);
    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, s.studentID) == 0) {
            printError("Student ID already exists. Please try a different ID.");
            return;
        }
    }

    inputPrompt("CGPA (0.0 - 4.0)");
    scanf("%f", &s.cgpa);
    getchar();

    if (s.cgpa < 0.0f || s.cgpa > 4.0f) {
        printError("CGPA must be between 0.0 and 4.0.");
        return;
    }

    inputPrompt("Department");
    fgets(s.department, sizeof(s.department), stdin);
    s.department[strcspn(s.department, "\n")] = 0;

    s.id       = generateNextID();
    s.isActive = 1;

    saveStudent(&s);

    printf("\n");
    boxTop();
    printf(C_BORDER BOX_V RESET "  " C_SUCCESS BOLD " Student registered successfully!" RESET
           "%*s" C_BORDER BOX_V RESET "\n", 15, "");
    printf(C_BORDER BOX_V RESET "  " C_INFO "Unique ID  " RESET ": "
           C_VALUE BOLD "%d" RESET
           "%*s" C_BORDER BOX_V RESET "\n", s.id, 33, "");
    printf(C_BORDER BOX_V RESET "  " C_INFO "Student ID " RESET ": "
           C_VALUE BOLD "%s" RESET
           "%*s" C_BORDER BOX_V RESET "\n", s.studentID, (int)(33 - strlen(s.studentID)), "");
    boxBottom();
    printf("\n");
}

/* ── View student profile ───────────────────────────────── */
void viewStudent(const char *studentID) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, studentID) == 0) {

            printf("\n");
            printBanner("  STUDENT PROFILE  ", 19);
            printf("\n");

            char idStr[12], cgpaStr[10];
            snprintf(idStr,   sizeof(idStr),   "%d",   arr[i].id);
            snprintf(cgpaStr, sizeof(cgpaStr), "%.2f", arr[i].cgpa);

            printKV("Name",       arr[i].name);
            printKV("Student ID", arr[i].studentID);
            printKV("Unique ID",  idStr);
            printKV("CGPA",       cgpaStr);
            printKV("Department", arr[i].department);
            printf("\n");

            found = 1;
            break;
        }
    }
    if (!found) {
        printError("Student ID not found.");
    }
}

/* ── Update student profile ─────────────────────────────── */
void updateStudent(const char *studentID) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, studentID) == 0) {

            printf("\n");
            printBanner("  UPDATE STUDENT PROFILE  ", 25);
            printf("\n");

            printf("  " C_DIM "Leave blank and press Enter to keep current value.\n" RESET "\n");

            /* Name */
            printf("  " C_INFO "Current Name       " RESET ": " C_VALUE "%s" RESET "\n",
                   arr[i].name);
            inputPrompt("New Name");
            char tmp[50];
            fgets(tmp, sizeof(tmp), stdin);
            tmp[strcspn(tmp, "\n")] = 0;
            if (strlen(tmp) > 0) {
                strncpy(arr[i].name, tmp, sizeof(arr[i].name) - 1);
            }

            /* CGPA */
            printf("  " C_INFO "Current CGPA       " RESET ": " C_VALUE "%.2f" RESET "\n",
                   arr[i].cgpa);
            inputPrompt("New CGPA");
            float newCgpa;
            if (scanf("%f", &newCgpa) == 1) {
                if (newCgpa < 0.0f || newCgpa > 4.0f) {
                    printError("CGPA must be between 0.0 and 4.0. Update aborted.");
                    getchar();
                    return;
                }
                arr[i].cgpa = newCgpa;
            }
            getchar();

            /* Department */
            printf("  " C_INFO "Current Department " RESET ": " C_VALUE "%s" RESET "\n",
                   arr[i].department);
            inputPrompt("New Department");
            fgets(tmp, sizeof(tmp), stdin);
            tmp[strcspn(tmp, "\n")] = 0;
            if (strlen(tmp) > 0) {
                strncpy(arr[i].department, tmp, sizeof(arr[i].department) - 1);
            }

            updateStudentFile(arr, n);
            printf("\n");
            printSuccess("Profile updated successfully!");
            found = 1;
            break;
        }
    }
    if (!found) {
        printError("Student ID not found. Update failed.");
    }
}

/* ── Delete student (soft) ──────────────────────────────── */
void deleteStudent(const char *studentID) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, studentID) == 0) {

            printf("\n  " C_WARNING SYM_WARN
                   "  Are you sure you want to delete student " RESET
                   C_VALUE BOLD "%s" RESET C_WARNING " ? (y/n): " RESET,
                   arr[i].name);

            char confirm = (char)getchar();
            getchar();

            if (confirm != 'y' && confirm != 'Y') {
                printInfo("Deletion cancelled.");
                return;
            }

            arr[i].isActive = 0;
            updateStudentFile(arr, n);
            printSuccess("Student deleted successfully.");
            found = 1;
            break;
        }
    }
    if (!found) {
        printError("Student ID not found. Deletion failed.");
    }
}

/* ── List all active students ───────────────────────────── */
void listAllStudents(void) {
    Student arr[100];
    int n, i;
    loadAllStudents(arr, &n);

    printf("\n");
    printBanner("  ALL ACTIVE STUDENTS  ", 22);
    printf("\n");

    /* Table header */
    printf("  " C_HEADER BOLD "%-4s  %-24s  %-12s  %-6s  %-14s" RESET "\n",
           "ID", "Name", "Student ID", "CGPA", "Department");
    printf("  " C_DIM);
    int j;
    for (j = 0; j < 66; j++) printf(BOX_H);
    printf(RESET "\n");

    int count = 0;
    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1) {
            /* Alternate row shading */
            if (count % 2 == 0)
                printf(RESET);
            else
                printf(DIM);

            printf("  " C_VALUE "%-4d" RESET "  %-24s  " C_ACCENT "%-12s" RESET
                   "  " C_INFO "%-6.2f" RESET "  %-14s\n",
                   arr[i].id, arr[i].name, arr[i].studentID,
                   arr[i].cgpa, arr[i].department);
            printf(RESET);
            count++;
        }
    }

    printf("  " C_DIM);
    for (j = 0; j < 66; j++) printf(BOX_H);
    printf(RESET "\n");

    if (count == 0) {
        printWarning("No active students found.");
    } else {
        printf("  " C_DIM "Total active students: " RESET C_VALUE BOLD "%d" RESET "\n\n",
               count);
    }
}

/* ── Search by name ─────────────────────────────────────── */
void searchByName(const char *name) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    printf("\n");
    printBanner("  SEARCH RESULTS  ", 18);
    printf("\n");

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strstr(arr[i].name, name) != NULL) {

            char idStr[12], cgpaStr[10];
            snprintf(idStr,   sizeof(idStr),   "%d",   arr[i].id);
            snprintf(cgpaStr, sizeof(cgpaStr), "%.2f", arr[i].cgpa);

            boxTop();
            printKV("Name",       arr[i].name);
            printKV("Student ID", arr[i].studentID);
            printKV("Unique ID",  idStr);
            printKV("CGPA",       cgpaStr);
            printKV("Department", arr[i].department);
            boxBottom();
            printf("\n");
            found = 1;
        }
    }
    if (!found) {
        printWarning("No students found matching that name.");
    }
}
