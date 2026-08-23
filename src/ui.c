#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "colors.h"
#include "auth.h"
#include "student.h"
#include "assessment.h"
#include "prediction.h"
#include "gap_analysis.h"
#include "report.h"
#include "analytics.h"
#include "db_handler.h"
#include "input_handler.h"

/* ══════════════════════════════════════════════════════════
   STATUS BAR — framed header rendered at top of every menu
   ══════════════════════════════════════════════════════════ */
void statusBar(Session s) {
    /* Top rule */
    putchar('\n');
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER));
    for (int i = 0; i < BOX_WIDTH + 4; i++) fputs(BOX_H, stdout);
    printf("%s\n", CC(RESET));

    /* Content row */
    fputs(UI_PAD " ", stdout);
    printf("%s%s%s", CC(C_PRIMARY), CC(BOLD), SYM_BOLT);
    printf("%s SYSTEM  %s|%s  ", CC(RESET), CC(C_BORDER2), CC(RESET));

    if (s.role == 0) {
        printf("%s%s%s Administrator  %s|%s  Master Control",
               CC(C_SUCCESS), CC(BOLD), SYM_DIAMOND,
               CC(C_BORDER2), CC(RESET));
    } else {
        Student cur;
        float cgpa = 0.0f;
        if (db_get_student_by_num(s.studentNumId, &cur)) cgpa = cur.cgpa;
        printf("%s%s%s Student  %s|%s  %s%s%s  %s|%s  %s%s  GPA: %s%.2f",
               CC(C_INFO), CC(BOLD), SYM_DIAMOND,
               CC(C_BORDER2), CC(RESET),
               CC(C_ACCENT), CC(BOLD), s.studentID,
               CC(C_BORDER2), CC(RESET),
               CC(C_WARNING), SYM_STAR,
               CC(C_VALUE), cgpa);
    }
    printf("%s\n", CC(RESET));

    /* Bottom rule */
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER));
    for (int i = 0; i < BOX_WIDTH + 4; i++) fputs(BOX_H, stdout);
    printf("%s\n\n", CC(RESET));
}

/* ══════════════════════════════════════════════════════════
   ADMIN MENU
   ══════════════════════════════════════════════════════════ */
void adminMenu(Session s) {
    while (1) {
        CLEAR_SCREEN();
        statusBar(s);

        const char *opts[] = {
            "View All Students",
            "Search Student by Name",
            "Delete a Student",
            "Analytics Dashboard",
            "Logout"
        };
        int sel = getMenuSelection("ADMINISTRATOR PANEL",
                                   "Restricted access area",
                                   opts, 5);

        if (sel == 0) {
            CLEAR_SCREEN();
            statusBar(s);
            listAllStudents();
            pauseScreen();

        } else if (sel == 1) {
            CLEAR_SCREEN();
            statusBar(s);
            printf("\n");
            printBanner("STUDENT SEARCH", 14);
            printf("\n");
            char query[50];
            inputPrompt("Enter name to search");
            getStringInput(query, sizeof(query));
            searchByName(query);
            pauseScreen();

        } else if (sel == 2) {
            CLEAR_SCREEN();
            statusBar(s);
            printf("\n");
            printBanner("DELETE STUDENT", 14);
            printf("\n");
            char delID[20];
            inputPrompt("Enter Student ID to delete");
            getStringInput(delID, sizeof(delID));
            deleteStudent(delID);
            pauseScreen();

        } else if (sel == 3) {
            CLEAR_SCREEN();
            statusBar(s);
            showAdminDashboard();

        } else if (sel == 4) {
            return;
        }
    }
}

/* ══════════════════════════════════════════════════════════
   STUDENT MENU
   ══════════════════════════════════════════════════════════ */
void studentMenu(Session s) {
    while (1) {
        CLEAR_SCREEN();
        statusBar(s);

        const char *opts[] = {
            "Update My Profile",
            "Take Skill Assessment",
            "View Career Prediction",
            "Skill Gap Analysis",
            "Generate My Report",
            "Logout"
        };
        int sel = getMenuSelection("STUDENT DASHBOARD",
                                   "Select an action to proceed",
                                   opts, 6);

        if (sel == 0) {
            CLEAR_SCREEN();
            statusBar(s);
            updateStudent(s.studentID);
            pauseScreen();

        } else if (sel == 1) {
            runAssessment(s.studentNumId);

        } else if (sel == 2) {
            SkillProfile sp;
            if (db_load_skill_profile(s.studentNumId, &sp)) {
                rankCareers(&sp);
            } else {
                printWarning("Complete the Skill Assessment first!");
                pauseScreen();
            }

        } else if (sel == 3) {
            SkillProfile sp;
            if (!db_load_skill_profile(s.studentNumId, &sp)) {
                printWarning("Complete the Skill Assessment first!");
                pauseScreen();
                continue;
            }

            /* Load the career the student was predicted for */
            PredictionRecord pr;
            if (!db_get_last_prediction(s.studentNumId, &pr)) {
                printWarning("View Career Prediction first to generate a result!");
                pauseScreen();
                continue;
            }

            /* Find that career in the careers list */
            CareerPath careers[NUM_CAREERS];
            int count = 0;
            getCareers(careers, &count);

            int foundIdx = -1;
            for (int i = 0; i < count; i++) {
                if (strcmp(careers[i].name, pr.topCareer) == 0) {
                    foundIdx = i;
                    break;
                }
            }
            if (foundIdx < 0) {
                printWarning("Career data not found. Please re-run Career Prediction.");
                pauseScreen();
                continue;
            }

            analyzeGap(&sp, &careers[foundIdx]);

        } else if (sel == 4) {
            SkillProfile sp;
            if (!db_load_skill_profile(s.studentNumId, &sp)) {
                printWarning("Complete the Skill Assessment first!");
                pauseScreen();
                continue;
            }
            Student st;
            if (!db_get_student_by_num(s.studentNumId, &st)) {
                printError("Could not load your profile.");
                pauseScreen();
                continue;
            }
            CareerPath careers[NUM_CAREERS];
            int count = 0;
            getCareers(careers, &count);
            float scores[NUM_CAREERS];
            for (int i = 0; i < count; i++)
                scores[i] = calculateScore(&sp, &careers[i]);
            /* Sort descending */
            for (int i = 0; i < count - 1; i++)
                for (int j = 0; j < count - 1 - i; j++)
                    if (scores[j] < scores[j+1]) {
                        float ts = scores[j]; scores[j] = scores[j+1]; scores[j+1] = ts;
                        CareerPath tp = careers[j]; careers[j] = careers[j+1]; careers[j+1] = tp;
                    }
            generateReport(&st, &sp, careers, scores);

        } else if (sel == 5) {
            return;
        }
    }
}
