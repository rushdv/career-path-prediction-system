#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "file_handler.h"
#include "student.h"

Session login(void)
{
    Session s;
    int choice;

    printf("\n +--------------------------------------------+\n");
    printf(" |        CAREER PATH PREDICTION SYSTEM        |\n");
    printf(" |        Norther University Bangladesh        |\n");
    printf(" +--------------------------------------------+\n");
    printf(" | 1. Admin Login                              |\n");
    printf(" | 2. Student Login                            |\n");
    printf(" | 3. Register as New Student                  |\n");
    printf(" | 4. Exit                                     |\n");
    printf(" +--------------------------------------------+\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); /* clear input buffer*/

    if (choice == 0)
    {
        printf(" Goodbye!\n");
        exit(EXIT_SUCCESS);
    }

    else if (choice == 1)
    {
        char pass[30];
        printf("Enter Admin Password: ");
        scanf("%s", pass);
        getchar();

        if (strcmp(pass, ADMIN_PASSWORD) != 0)
        {
            printf(" Access Denied! Invalid Password.\n");
            exit(EXIT_FAILURE);
        }
        s.role = 0; /* Admin role */
        strcpy(s.studentID, "ADMIN");
        printf(" Access Granted! Welcome Admin.\n");
    }

    else if (choice == 2)
    {
        printf("Enter your Student ID: ");
        scanf("%s", s.studentID);
        getchar();

        /* verify student exists in file */
        Student arr[100];
        int n, found = 0;
        loadALLStudents(arr, &n);

        int i;
        for (i = 0; i < n; i++)
        {
            if (arr[i].isActive == 1 &&
                strcmp(arr[i].studentID, s.studentID) == 0)
            {
                found = 1;
                printf(" Access Granted! Welcome %s.\n", arr[i].name);
                break;
            }
        }

        if (!found)
        {
            printf(" Access Denied! Student ID not found. Please register first.\n");
            exit(EXIT_FAILURE);
        }
        s.role = 1; /* Student role */
    }

    else if (choice == 3)
    {
        /* New student registers then logs in */
        s.role = 1;
        createStudent();
        printf(" Registration successful! Now login with your new Student ID.\n");
        printf("Enter your Student ID: ");
        scanf("%s", s.studentID);
        getchar();
    }

    else
    {
        printf(" Invalid choice! Exiting.\n");
        exit(EXIT_FAILURE);
    }

    return s;
}