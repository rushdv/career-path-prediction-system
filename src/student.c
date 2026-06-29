#include <stdio.h>
#include <string.h>
#include "student.h"
#include "file_handler.h"

/* Generate next unique student ID */
static int generateNextID(void) {
    Student arr[100];
    int n;
    loadAllStudents(arr, &n);
    if (n == 0) return 1; /* If no students, start with ID 1 */
    return arr[n - 1].id + 1; /* Return last ID + 1 */
}

/* Create a new student profile */
void createStudent(void) {
    Student s;

    printf("\n +--------------------------------------------+\n");
    printf(" |          CREATE NEW STUDENT PROFILE        |\n");
    printf(" +--------------------------------------------+\n");

    printf("Enter Student Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0; /* Remove newline character */

    printf("Enter Student ID: ");
    scanf("%s", s.studentID);
    getchar();

    /* Check if student ID already exists */
    Student arr[100];
    int n, i;
    loadAllStudents(arr, &n);
    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, s.studentID) == 0) {
            printf("Error: Student ID already exists. Please try again.\n");
            return;
        }
    }

    printf( CGPA (0.0 - 4.0): ");
    scanf("%f", &s.cgpa);
    getchar();

    if (s.cgpa < 0.0f || s.cgpa > 4.0f) {
        printf("Error: CGPA must be between 0.0 and 4.0. Please try again.\n");
        return;
    }

    printf(" Enter Department: ");
    fgets(s.department, sizeof(s.department), stdin);
    s.department[strcspn(s.department, "\n")] = 0; /* Remove newline character */

    s.id = generateNextID();
    s.isActive = 1; /* Mark as active */

    /* Save the new student to file */
    saveStudent(&s);
    printf("Student profile created successfully! Your unique ID is: %d\n", s.id);
}


/* View a student profile by student ID */
void viewStudent(const char *studentID) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, studentID) == 0) {
            
            printf("\n +--------------------------------------------+\n");
            printf(" |          STUDENT PROFILE DETAILS           |\n");
            printf(" +--------------------------------------------+\n");
            printf(" ID         : %d\n",   arr[i].id);
            printf(" Name       : %s\n",   arr[i].name);
            printf(" Student ID : %s\n",   arr[i].studentID);
            printf(" CGPA       : %.2f\n", arr[i].cgpa);
            printf(" Department : %s\n",   arr[i].department);
            printf(" +--------------------------------------------+\n");

            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: Student ID not found.\n");
    }
}


/* Update a student profile */
void updateStudent(const char *studentID) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, studentID) == 0) {
            
            printf("\n +--------------------------------------------+\n");
            printf(" |          UPDATE STUDENT PROFILE            |\n");
            printf(" +--------------------------------------------+\n");

            printf("Current Name       : %s\n", arr[i].name);
            printf("Enter New Name     : ");
            fgets(arr[i].name, sizeof(arr[i].name), stdin);
            arr[i].name[strcspn(arr[i].name, "\n")] = 0; 
            if (strlen(arr[i].name) == 0) {
                printf(" Name unchanged.\n");
                loadAllStudents(arr, &n); /* Reload to keep original name */
                continue; /* Skip to next iteration */
                }


            printf("Current CGPA       : %.2f\n", arr[i].cgpa);
            printf("Enter New CGPA     : ");
            scanf("%f", &arr[i].cgpa);
            getchar();

            if (arr[i].cgpa < 0.0f || arr[i].cgpa > 4.0f) {
                printf("Error: CGPA must be between 0.0 and 4.0. Update aborted.\n");
                return;
            }

            printf("Current Department : %s\n", arr[i].department);
            printf("Enter New Department: ");
            fgets(arr[i].department, sizeof(arr[i].department), stdin);
            arr[i].department[strcspn(arr[i].department, "\n")] = 0; 

            /* Save the updated student to file */
            updateStudentFile(&arr[i]);
            printf("Student profile updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: Student ID not found. Update failed.\n");
    }
}


/* Soft delete a student */
void deleteStudent(const char *studentID) {
    Student arr[100];
    int n, i, found = 0;
    loadAllStudents(arr, &n);

    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1 &&
            strcmp(arr[i].studentID, studentID) == 0) {
            
            arr[i].isActive = 0; /* Mark as deleted */
            updateStudentFile(&arr[i]);
            printf("Student profile deleted successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: Student ID not found. Deletion failed.\n");
    }
}

/* List all active students (for admin view) */
void listAllStudents(void) {
    Student arr[100];
    int n, i;
    loadAllStudents(arr, &n);


    printf("\n +--------------------------------------------+\n");
    printf(" |          LIST OF ALL ACTIVE STUDENTS       |\n");
    printf(" +--------------------------------------------+\n");
    printf(" ID   | Name                     | Student ID | CGPA | Department\n");
    printf("---------------------------------------------------------------\n");

    int count = 0;
    for (i = 0; i < n; i++) {
        if (arr[i].isActive == 1) {
            printf(" %-4d | %-24s | %-10s | %.2f | %-10s\n",
                   arr[i].id, arr[i].name, arr[i].studentID, arr[i].cgpa, arr[i].department);
            count++;
        }
    }

    if (count == 0) {
        printf(" No active students found.\n");
    }
    printf(" +--------------------------------------------+\n");
    printf(" Total Active Students: %d\n", count);
}