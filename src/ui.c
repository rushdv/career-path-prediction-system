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
#include "input_handler.h"
#include "analytics.h"

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
    const char *options[] = {
        "View All Students",
        "View Student Profile",
        "Update Student Profile",
        "Delete Student",
        "Search Student by Name",
        "View Prediction History",
        "View Analytics Dashboard",
        "Export Data to CSV",
        "Logout"
    };
    do {
        int sel = getMenuSelection("ADMIN DASHBOARD", NULL, options, 9);
        if (sel == 8) choice = 0;
        else choice = sel + 1;

        if (choice != 0) handleChoice(choice, &session, NULL, NULL);

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

    /* ── Skill profile state lives here, not inside handleChoice ── */
    SkillProfile sp;
    int hasAssessment = 0;

    /* Auto-load saved assessment from previous session */
    if (session->studentNumId > 0 &&
        loadSkillProfile(session->studentNumId, &sp)) {
        hasAssessment = 1;
    }

    const char *options[] = {
        "View My Profile",
        "Update My Profile",
        "Run Skill Assessment",
        "View Career Prediction",
        "View Gap Analysis",
        "Generate Report",
        "View My Prediction History",
        "Logout"
    };

    do {
        char subtitle[256];
        if (hasAssessment) {
            snprintf(subtitle, sizeof(subtitle), "  " C_SUCCESS BOLD SYM_CHECK RESET "  " C_DIM "Skill profile loaded" RESET);
        } else {
            snprintf(subtitle, sizeof(subtitle), "  " C_WARNING BOLD SYM_WARN RESET "  " C_DIM "No skill profile — run option 3 first" RESET);
        }

        int sel = getMenuSelection("STUDENT DASHBOARD", subtitle, options, 8);
        if (sel == 7) choice = 0;
        else choice = sel + 1;

        if (choice != 0) handleChoice(choice, session, &sp, &hasAssessment);

    } while (choice != 0);

    printf("\n");
    printInfo("Logged out. See you next time!");
    printf("\n");
}

/* ══════════════════════════════════════════════════════════
   HANDLE CHOICE DISPATCHER
   ══════════════════════════════════════════════════════════ */
void handleChoice(int choice, Session *session, SkillProfile *sp, int *hasAssessment) {

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
                getStringInput(sid, sizeof(sid));

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

            case 7:
                showAdminDashboard();
                break;

            case 8:
                exportStudentsToCSV();
                break;

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

                /* Warn before overwriting an existing assessment */
                if (*hasAssessment) {
                    printf("\n  " C_WARNING BOLD SYM_WARN RESET
                           "  " C_WARNING "You already have a saved assessment. Overwrite? (y/n): " RESET);
                    char confirm = (char)getchar();
                    getchar();
                    if (confirm != 'y' && confirm != 'Y') {
                        printInfo("Assessment kept unchanged.");
                        break;
                    }
                }

                *sp = runAssessment(me->id);
                *hasAssessment = 1;
                saveSkillProfile(sp);          /* persist to data/skills.dat */
                printSuccess("Skill profile saved!");
                break;

            case 4:
                if (!*hasAssessment) {
                    printWarning("Please run the Skill Assessment first (option 3).");
                    pauseScreen(); break;
                }
                rankCareers(sp);
                break;

            case 5: {
                if (!*hasAssessment) {
                    printWarning("Please run the Skill Assessment first (option 3).");
                    pauseScreen(); break;
                }
                CareerPath cars[NUM_CAREERS];
                float      scs[NUM_CAREERS];
                int        count, j;
                getCareers(cars, &count);
                for (j = 0; j < count; j++) scs[j] = calculateScore(sp, &cars[j]);

                int best = 0;
                for (j = 1; j < count; j++)
                    if (scs[j] > scs[best]) best = j;

                analyzeGap(sp, &cars[best]);
                break;
            }

            case 6: {
                if (!*hasAssessment) {
                    printWarning("Please run the Skill Assessment first (option 3).");
                    pauseScreen(); break;
                }
                if (me == NULL) { printError("Student profile not found."); pauseScreen(); break; }

                CareerPath cars[NUM_CAREERS];
                float      scs[NUM_CAREERS];
                int        count, j;
                getCareers(cars, &count);
                for (j = 0; j < count; j++) scs[j] = calculateScore(sp, &cars[j]);

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
                generateReport(me, sp, cars, scs);
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
