#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "colors.h"
#include "student.h"
#include "assessment.h"
#include "prediction.h"
#include "gap_analysis.h"
#include "report.h"
#include "file_handler.h"

/* ── Shared: print history table ────────────────────────── */
static void printHistory(int studentRef, const char *heading) {
    PredictionRecord hist[50];
    int hcount = 0, i;
    loadHistory(studentRef, hist, &hcount);

    printf("\n");
    printBanner(heading, (int)strlen(heading));
    printf("\n");

    if (hcount == 0) {
        printWarning("No prediction history found.");
        return;
    }

    printf("  " C_HEADER BOLD "%-4s  %-20s  %-24s  %s\n" RESET,
           "#", "Date & Time", "Top Career", "Score");
    printf("  " C_DIM);
    int d; for (d = 0; d < 62; d++) printf(BOX_H);
    printf(RESET "\n");

    for (i = 0; i < hcount; i++) {
        printf("  " C_VALUE "%-4d" RESET "  "
               C_DIM  "%-20s" RESET "  "
               C_ACCENT "%-24s" RESET "  "
               C_SUCCESS BOLD "%.2f/10" RESET "\n",
               i + 1, hist[i].timestamp,
               hist[i].topCareer, hist[i].score);
    }

    printf("  " C_DIM);
    for (d = 0; d < 62; d++) printf(BOX_H);
    printf(RESET "\n\n");
}

/* ══════════════════════════════════════════════════════════
   ADMIN MENU
   ══════════════════════════════════════════════════════════ */
void showAdminMenu(void) {
    Session session;
    session.role = 0;
    strcpy(session.studentID, "ADMIN");

    int choice;
    do {
        CLEAR_SCREEN();
        printf("\n");
        printBanner("  ADMIN DASHBOARD  ", 19);
        printf("\n");

        printf("  " C_MENU_NUM BOLD "1." RESET "  " C_MENU_TEXT "View All Students\n"      RESET);
        printf("  " C_MENU_NUM BOLD "2." RESET "  " C_MENU_TEXT "View Student Profile\n"   RESET);
        printf("  " C_MENU_NUM BOLD "3." RESET "  " C_MENU_TEXT "Update Student Profile\n" RESET);
        printf("  " C_MENU_NUM BOLD "4." RESET "  " C_MENU_TEXT "Delete Student\n"         RESET);
        printf("  " C_MENU_NUM BOLD "5." RESET "  " C_MENU_TEXT "Search Student by Name\n" RESET);
        printf("  " C_MENU_NUM BOLD "6." RESET "  " C_MENU_TEXT "View Prediction History\n"RESET);
        printf("\n");
        printf("  " C_ERROR   BOLD "0." RESET "  " C_DIM "Logout\n" RESET);
        printf("\n");

        inputPrompt("Enter choice");
        scanf("%d", &choice);
        getchar();

        if (choice != 0) handleChoice(choice, &session);

    } while (choice != 0);

    printf("\n");
    printInfo("Logged out. Goodbye, Admin!");
    printf("\n");
}

/* ══════════════════════════════════════════════════════════
   STUDENT MENU
   ══════════════════════════════════════════════════════════ */
void showStudentMenu(Session *session) {
    int choice;
    do {
        CLEAR_SCREEN();
        printf("\n");
        printBanner("  STUDENT DASHBOARD  ", 21);
        printf("\n");

        printf("  " C_MENU_NUM BOLD "1." RESET "  " C_MENU_TEXT "View My Profile\n"          RESET);
        printf("  " C_MENU_NUM BOLD "2." RESET "  " C_MENU_TEXT "Update My Profile\n"        RESET);
        printf("  " C_MENU_NUM BOLD "3." RESET "  " C_MENU_TEXT "Run Skill Assessment\n"     RESET);
        printf("  " C_MENU_NUM BOLD "4." RESET "  " C_MENU_TEXT "View Career Prediction\n"   RESET);
        printf("  " C_MENU_NUM BOLD "5." RESET "  " C_MENU_TEXT "View Gap Analysis\n"        RESET);
        printf("  " C_MENU_NUM BOLD "6." RESET "  " C_MENU_TEXT "Generate Report\n"          RESET);
        printf("  " C_MENU_NUM BOLD "7." RESET "  " C_MENU_TEXT "View My Prediction History\n"RESET);
        printf("\n");
        printf("  " C_ERROR   BOLD "0." RESET "  " C_DIM "Logout\n" RESET);
        printf("\n");

        inputPrompt("Enter choice");
        scanf("%d", &choice);
        getchar();

        if (choice != 0) handleChoice(choice, session);

    } while (choice != 0);

    printf("\n");
    printInfo("Logged out. See you next time!");
    printf("\n");
}

/* ══════════════════════════════════════════════════════════
   HANDLE CHOICE DISPATCHER
   ══════════════════════════════════════════════════════════ */
void handleChoice(int choice, Session *session) {

    /* ── ADMIN ───────────────────────────────────────────── */
    if (session->role == 0) {
        char sid[20];
        switch (choice) {

            case 1:
                listAllStudents();
                pauseScreen();
                break;

            case 2:
                printf("\n");
                inputPrompt("Enter Student ID to view");
                scanf("%s", sid); getchar();
                viewStudent(sid);
                pauseScreen();
                break;

            case 3:
                printf("\n");
                inputPrompt("Enter Student ID to update");
                scanf("%s", sid); getchar();
                updateStudent(sid);
                pauseScreen();
                break;

            case 4:
                printf("\n");
                inputPrompt("Enter Student ID to delete");
                scanf("%s", sid); getchar();
                deleteStudent(sid);
                pauseScreen();
                break;

            case 5:
                printf("\n");
                inputPrompt("Enter name to search");
                fgets(sid, sizeof(sid), stdin);
                sid[strcspn(sid, "\n")] = 0;
                searchByName(sid);
                pauseScreen();
                break;

            case 6: {
                printf("\n");
                inputPrompt("Enter Student ID to view history");
                scanf("%s", sid); getchar();

                Student arr[100];
                int n, i;
                loadAllStudents(arr, &n);
                int ref = -1;
                for (i = 0; i < n; i++) {
                    if (strcmp(arr[i].studentID, sid) == 0) {
                        ref = arr[i].id;
                        break;
                    }
                }
                if (ref == -1) {
                    printError("Student not found.");
                } else {
                    printHistory(ref, "  PREDICTION HISTORY  ");
                }
                pauseScreen();
                break;
            }

            default:
                printError("Invalid choice. Please try again.");
                pauseScreen();
        }
        return;
    }

    /* ── STUDENT ─────────────────────────────────────────── */
    if (session->role == 1) {
        Student arr[100];
        int n, i;
        loadAllStudents(arr, &n);
        Student *me = NULL;
        for (i = 0; i < n; i++) {
            if (arr[i].isActive == 1 &&
                strcmp(arr[i].studentID, session->studentID) == 0) {
                me = &arr[i];
                break;
            }
        }

        static SkillProfile sp;
        static int hasAssessment = 0;

        switch (choice) {

            case 1:
                viewStudent(session->studentID);
                pauseScreen();
                break;

            case 2:
                updateStudent(session->studentID);
                pauseScreen();
                break;

            case 3:
                if (me == NULL) { printError("Student profile not found."); pauseScreen(); break; }
                sp = runAssessment(me->id);
                hasAssessment = 1;
                break;

            case 4:
                if (!hasAssessment) {
                    printWarning("Please run the Skill Assessment first (option 3).");
                    pauseScreen(); break;
                }
                rankCareers(&sp);
                break;

            case 5: {
                if (!hasAssessment) {
                    printWarning("Please run the Skill Assessment first (option 3).");
                    pauseScreen(); break;
                }
                CareerPath cars[NUM_CAREERS];
                float      scs[NUM_CAREERS];
                int        count, j;
                getCareers(cars, &count);
                for (j = 0; j < count; j++) scs[j] = calculateScore(&sp, &cars[j]);

                int best = 0;
                for (j = 1; j < count; j++)
                    if (scs[j] > scs[best]) best = j;

                analyzeGap(&sp, &cars[best]);
                break;
            }

            case 6: {
                if (!hasAssessment) {
                    printWarning("Please run the Skill Assessment first (option 3).");
                    pauseScreen(); break;
                }
                if (me == NULL) { printError("Student profile not found."); pauseScreen(); break; }

                CareerPath cars[NUM_CAREERS];
                float      scs[NUM_CAREERS];
                int        count, j;
                getCareers(cars, &count);
                for (j = 0; j < count; j++) scs[j] = calculateScore(&sp, &cars[j]);

                /* Bubble sort descending */
                for (j = 0; j < count - 1; j++) {
                    int k;
                    for (k = 0; k < count - 1 - j; k++) {
                        if (scs[k] < scs[k+1]) {
                            float      ts = scs[k];   scs[k]   = scs[k+1];  scs[k+1]  = ts;
                            CareerPath tc = cars[k];  cars[k]  = cars[k+1]; cars[k+1] = tc;
                        }
                    }
                }
                generateReport(me, &sp, cars, scs);
                break;
            }

            case 7:
                if (me == NULL) { printError("Student profile not found."); pauseScreen(); break; }
                printHistory(me->id, "  MY PREDICTION HISTORY  ");
                pauseScreen();
                break;

            default:
                printError("Invalid choice. Please try again.");
                pauseScreen();
        }
    }
}
