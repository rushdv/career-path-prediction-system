#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "colors.h"
#include "db_handler.h"
#include "student.h"
#include "input_handler.h"

/* ══════════════════════════════════════════════════════════
   SPLASH SCREEN — centred double-line frame
   ══════════════════════════════════════════════════════════ */
static void splashRow(const char *content, int frameWidth) {
    int contentVisible = utf8_display_len(content);
    int lpad = (frameWidth - contentVisible) / 2;
    int rpad = frameWidth - contentVisible - lpad;
    fputs(UI_PAD, stdout);
    printf("%s", CC(C_BORDER)); fputs(BOX_V, stdout); printf("%s", CC(RESET));
    printf("%*s", lpad, "");
    fputs(content, stdout);
    printf("%*s", rpad, "");
    printf("%s", CC(C_BORDER)); fputs(BOX_V, stdout); printf("%s\n", CC(RESET));
}

static void drawSplash(void) {
    CLEAR_SCREEN();
    printf("\n\n");

    int frameWidth = BOX_WIDTH + 4;   /* inner visible chars between the ║ borders */

    /* Top border */
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER));
    fputs(BOX_TL, stdout);
    for (int i = 0; i < frameWidth; i++) fputs(BOX_H, stdout);
    fputs(BOX_TR, stdout); printf("%s\n", CC(RESET));

    /* Empty row */
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER)); fputs(BOX_V, stdout);
    printf("%*s", frameWidth, "");
    fputs(BOX_V, stdout); printf("%s\n", CC(RESET));

    /* System title  — bolt + text + bolt */
    {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s%s%s %s %s%s",
                 CC(C_ACCENT), CC(BOLD), SYM_BOLT,
                 "CAREER PATH PREDICTION SYSTEM",
                 SYM_BOLT,CC(RESET));
        splashRow(buf, frameWidth);
    }

    /* Institution 
    {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s%s%s",
                 CC(C_INFO), " University ", CC(RESET));
        splashRow(buf, frameWidth);
    }
    */
    /* Version */
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s%s%s", CC(C_DIM), "v2.0  |  2026 Edition", CC(RESET));
        splashRow(buf, frameWidth);
    }

    /* Empty row */
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER)); fputs(BOX_V, stdout);
    printf("%*s", frameWidth, "");
    fputs(BOX_V, stdout); printf("%s\n", CC(RESET));

    /* Powered-by tag */
    {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s%s%s",
                 CC(C_MUTED), "= Predictive AI Analytics Engine =", CC(RESET));
        splashRow(buf, frameWidth);
    }

    /* Empty row */
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER)); fputs(BOX_V, stdout);
    printf("%*s", frameWidth, "");
    fputs(BOX_V, stdout); printf("%s\n", CC(RESET));

    /* Bottom border */
    fputs(UI_PAD, stdout); printf("%s", CC(C_BORDER));
    fputs(BOX_BL, stdout);
    for (int i = 0; i < frameWidth; i++) fputs(BOX_H, stdout);
    fputs(BOX_BR, stdout); printf("%s\n\n", CC(RESET));
}

/* ══════════════════════════════════════════════════════════
   LOGIN ENTRY POINT
   ══════════════════════════════════════════════════════════ */
Session login(void) {
    Session s;
    memset(&s, 0, sizeof(Session));

    drawSplash();

    const char *menuOptions[] = {
        "Admin Login",
        "Student Login",
        "Register as New Student",
        "Exit"
    };
    int sel = getMenuSelection("AUTHENTICATION",
                               "Powered by Predictive AI Analytics Engine",
                               menuOptions, 4);

    /* ── Exit ─────────────────────────────────────────────── */
    if (sel == 3) {
        printf("\n" UI_PAD "%sGoodbye! See you next time.%s\n\n", CC(C_DIM), CC(RESET));
        exit(EXIT_SUCCESS);
    }

    /* ── Admin Login ──────────────────────────────────────── */
    if (sel == 0) {
        CLEAR_SCREEN();
        printf("\n");
        printBanner("ADMIN AUTHENTICATION", 20);
        printf("\n");

        char pass[64];
        inputPrompt("Admin Password");
        printf("\033[8m");
        if (fgets(pass, sizeof(pass), stdin) != NULL)
            pass[strcspn(pass, "\r\n")] = 0;
        printf("\033[28m");

        if (strcmp(pass, ADMIN_PASSWORD) != 0) {
            printError("Access denied. Invalid administrator password.");
            pauseScreen();
            exit(EXIT_FAILURE);
        }
        s.role = 0;
        strcpy(s.studentID, "ADMIN");
        s.studentNumId = 0;
        printSuccess("Access granted. Welcome, Administrator.");
        pauseScreen();
    }

    /* ── Student Login ────────────────────────────────────── */
    else if (sel == 1) {
        CLEAR_SCREEN();
        printf("\n");
        printBanner("STUDENT LOGIN", 13);
        printf("\n");

        inputPrompt("Student ID");
        if (fgets(s.studentID, sizeof(s.studentID), stdin) != NULL)
            s.studentID[strcspn(s.studentID, "\r\n")] = 0;

        char pass[64];
        char passHash[65];
        inputPrompt("Password");
        printf("\033[8m");
        if (fgets(pass, sizeof(pass), stdin) != NULL)
            pass[strcspn(pass, "\r\n")] = 0;
        printf("\033[28m");

        hashPassword(pass, passHash);

        Student current;
        if (db_get_student_by_id(s.studentID, &current) && current.isActive) {
            if (strcmp(current.password, passHash) != 0) {
                printError("Access denied. Incorrect password.");
                pauseScreen();
                exit(EXIT_FAILURE);
            }
            s.studentNumId = current.id;
            s.role         = 1;
            printf("\n");
            printSuccess("Access granted!");
            printf(UI_PAD "%sWelcome back, %s%s%s%s%s\n\n",
                   CC(C_INFO), CC(RESET), CC(C_ACCENT), CC(BOLD), current.name, CC(RESET));
            pauseScreen();
        } else {
            printError("Student ID not found. Please register first.");
            pauseScreen();
            exit(EXIT_FAILURE);
        }
    }

    /* ── Register ─────────────────────────────────────────── */
    else if (sel == 2) {
        s.role = 1;
        createStudent();
        printInfo("Registration complete! Please log in with your Student ID.");
        printf("\n");
        inputPrompt("Enter your Student ID");
        getStringInput(s.studentID, sizeof(s.studentID));

        Student temp;
        if (db_get_student_by_id(s.studentID, &temp)) {
            s.studentNumId = temp.id;
        } else {
            s.studentNumId = 0;
        }
    }

    else {
        printError("Invalid choice.");
        exit(EXIT_FAILURE);
    }

    return s;
}
