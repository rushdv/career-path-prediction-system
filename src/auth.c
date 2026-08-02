#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "colors.h"
#include "file_handler.h"
#include "student.h"

Session login(void)
{
    Session s;
    int choice;

    CLEAR_SCREEN();

    /* ── Splash / ASCII art ──────────────────────────────── */
    printf("\n");
    printf(C_BORDER "  " DBL_TL DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_TR RESET "\n");

    printf(C_BORDER "  " DBL_V RESET
           "                                                " C_BORDER DBL_V RESET "\n");

    printf(C_BORDER "  " DBL_V RESET
           C_TITLE BOLD "     \xe2\x97\x8f  CAREER PATH PREDICTION SYSTEM  \xe2\x97\x8f     " RESET
           C_BORDER DBL_V RESET "\n");

    printf(C_BORDER "  " DBL_V RESET
           C_DIM "         Northern University Bangladesh          " RESET
           C_BORDER DBL_V RESET "\n");

    printf(C_BORDER "  " DBL_V RESET
           "                                                " C_BORDER DBL_V RESET "\n");

    printf(C_BORDER "  " DBL_BL DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H DBL_H
           DBL_H DBL_H DBL_H DBL_H DBL_BR RESET "\n\n");

    /* ── Login menu box ─────────────────────────────────── */
    printf(C_BORDER "  " BOX_TL);
    int _i; for (_i = 0; _i < 48; _i++) printf(BOX_H);
    printf(BOX_TR RESET "\n");

    printf(C_BORDER "  " BOX_V RESET "  "
           C_MENU_NUM BOLD "1." RESET "  " C_MENU_TEXT "Admin Login                              " RESET
           C_BORDER BOX_V RESET "\n");

    printf(C_BORDER "  " BOX_V RESET "  "
           C_MENU_NUM BOLD "2." RESET "  " C_MENU_TEXT "Student Login                            " RESET
           C_BORDER BOX_V RESET "\n");

    printf(C_BORDER "  " BOX_V RESET "  "
           C_MENU_NUM BOLD "3." RESET "  " C_MENU_TEXT "Register as New Student                  " RESET
           C_BORDER BOX_V RESET "\n");

    printf(C_BORDER "  " BOX_V RESET "  "
           C_MENU_NUM BOLD "0." RESET "  " C_MENU_TEXT "Exit                                     " RESET
           C_BORDER BOX_V RESET "\n");

    printf(C_BORDER "  " BOX_BL);
    for (_i = 0; _i < 48; _i++) printf(BOX_H);
    printf(BOX_BR RESET "\n\n");

    inputPrompt("Enter your choice");
    scanf("%d", &choice);
    getchar();

    /* ── Handlers ────────────────────────────────────────── */
    if (choice == 0)
    {
        printf("\n  " C_DIM "Goodbye! See you next time." RESET "\n\n");
        exit(EXIT_SUCCESS);
    }

    else if (choice == 1)
    {
        char pass[30];
        printf("\n");
        boxTop();
        printf(C_BORDER BOX_V RESET "   " C_TITLE BOLD "  ADMIN AUTHENTICATION  " RESET
               "%*s" C_BORDER BOX_V RESET "\n", 23, "");
        boxBottom();

        printf("\n");
        inputPrompt("Enter Admin Password");
        /* Hide password input */
        printf("\033[8m"); /* conceal */
        scanf("%s", pass);
        printf("\033[28m"); /* reveal */
        getchar();

        if (strcmp(pass, ADMIN_PASSWORD) != 0)
        {
            printError("Access Denied! Invalid password.");
            exit(EXIT_FAILURE);
        }
        s.role = 0;
        strcpy(s.studentID, "ADMIN");
        printf("\n");
        printSuccess("Access Granted! Welcome, Admin.");
        pauseScreen();
    }

    else if (choice == 2)
    {
        printf("\n");
        boxTop();
        printf(C_BORDER BOX_V RESET "   " C_TITLE BOLD "  STUDENT AUTHENTICATION  " RESET
               "%*s" C_BORDER BOX_V RESET "\n", 21, "");
        boxBottom();

        printf("\n");
        inputPrompt("Enter your Student ID");
        scanf("%s", s.studentID);
        getchar();

        Student arr[100];
        int n = 0, found = 0, i;
        loadAllStudents(arr, &n);

        for (i = 0; i < n; i++)
        {
            if (arr[i].isActive == 1 &&
                strcmp(arr[i].studentID, s.studentID) == 0)
            {
                found = 1;
                printf("\n");
                printSuccess("Access Granted!");
                printf("  " C_INFO "Welcome back, " RESET C_VALUE BOLD "%s" RESET "\n\n",
                       arr[i].name);
                pauseScreen();
                break;
            }
        }

        if (!found)
        {
            printError("Access Denied! Student ID not found. Please register first.");
            exit(EXIT_FAILURE);
        }
        s.role = 1;
    }

    else if (choice == 3)
    {
        s.role = 1;
        createStudent();
        printInfo("Registration complete! Now login with your Student ID.");
        printf("\n");
        inputPrompt("Enter your Student ID");
        scanf("%s", s.studentID);
        getchar();
    }

    else
    {
        printError("Invalid choice. Exiting.");
        exit(EXIT_FAILURE);
    }

    return s;
}
